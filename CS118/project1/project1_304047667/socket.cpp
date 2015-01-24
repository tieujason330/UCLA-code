#include "socket.h"


/* Constructor to instantiate the socket
   Set sockfd to SOCKET_DEFAULT to keep track of it. 
*/
Socket::Socket()
{
	m_sockfd = SOCKET_DEFAULT;
	memset(&m_sock_addr, 0, sizeof(m_sock_addr));
}

/* Destructor, calls close on socket 
*/
Socket::~Socket() 
{
	if(m_sockfd != SOCKET_DEFAULT)
		close(m_sockfd);
}

/* Creates socket and update our socketfd (file descriptor)
   returns 0 on success, -1 on failure 
*/
int Socket::create_soc()
{
	/* int socket(int domain, int type, int protocol)
           domain: protocol family
           type: communication style (SOCK_STREAM for TCP)
           protocol: typically set to 0 */
	m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(m_sockfd != SOCKET_DEFAULT) return 0;
	return -1; //didn't allocate sockfd
}

/* Bind socket to a local IP addr and port number
   returns 0 on success, -1 on failure 
*/
int Socket::bind_soc(int port_num)
{
	m_sock_addr.sin_family = AF_INET; //protocol family
	//htons - host-to-net short (16-bit) translation
	//needed before sending data
	m_sock_addr.sin_port = htons(port_num); //port number assigned by caller
	m_sock_addr.sin_addr.s_addr = INADDR_ANY; //any IP can connect
	
	/* int bind(int sockfd, struct sockaddr* myaddr, int addrlen) 
	   addrlen: sizeof(struct sockaddr_in) */
	if(bind(m_sockfd, (struct sockaddr*) &m_sock_addr, sizeof(m_sock_addr)) == 0)
		return 0;
	return -1;
}

/* Put socket into passive state (wait for connections) 
   returns 0 on success, -1 on failure
*/
int Socket::listen(int backlog)
{
	/* int listen(int sockfd, int backlog)
	   backlog = max num of connections this program can serve simult */
	if( ::listen(m_sockfd, backlog) == 0) return 0;
	return -1; 
}

/* Accepts a new connection
   returns 0 on success, and -1 on failure
*/
int Socket::accept(Socket& new_socket)
{
	/* int accept(int sockfd, struct sockaddr* client_addr, int* addrlen)
	   addrlen = sizeof(struct sockaddr_in)
	*/
	int addrlen = sizeof(m_sock_addr);
	new_socket.m_sockfd = ::accept(m_sockfd, (sockaddr *) &m_sock_addr, (socklen_t *) &addrlen);

	if(new_socket.m_sockfd != SOCKET_DEFAULT) return 0;
	return -1;
}

/* Write data function 
   returns 0 on success, and -1 on failure
*/
int Socket::write_data(std::string d)
{
	int x = write(m_sockfd, d.c_str(), d.size()); 
	//std::cout << "****** " << x << " *******" << std::endl;
	if(x != -1) return 0;
	return -1;
}

/* Read data function
   returns 0 on success, and -1 on failure
*/
int Socket::read_data(std::string& d)
{
	char buf[MAX_REQUEST_SIZE+1];
	memset(buf, '\0', sizeof(buf));
	int recv_stat = read(m_sockfd, buf, MAX_REQUEST_SIZE);

	if(recv_stat == -1 || recv_stat == 0)
	{
		d = "";
		return -1;
	}
	d = buf;
	return 0;
}

/* Returns port num that socket is bound to
   Returns -1 on failure 
*/
int Socket::sock_port_number()
{
	sockaddr_in address;
	socklen_t address_length = sizeof(address);
	if (getsockname(m_sockfd, (struct sockaddr *) &address, &address_length) != -1)
		//call ntohs before reading received data
		return ntohs (address.sin_port); //ntohs - net-to-host short (16-bit) translation
	return -1;
}



