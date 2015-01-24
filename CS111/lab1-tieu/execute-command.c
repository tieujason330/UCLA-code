#define _GNU_SOURCE

#include "alloc.h"
#include "command.h"
#include "command-internals.h"

#include <errno.h>
#include <error.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

typedef struct dependency_node *d_node_;
typedef struct file_node *f_node_;
typedef struct tid_node *t_node_;

d_node_ d_root;

enum dependencyType {
  READ_F,
  WRITE_F,
};

struct dependency_node {
  char *fileName;
  t_node_ waitList;
  d_node_ next;
};

struct file_node {
  char *fileName;
  enum dependencyType type;
  f_node_ next;
  f_node_ prev;
};

struct tid_node {
  pthread_t tid;
  enum dependencyType type;
  t_node_ next;
};

int
command_status(command_t c)
{
  return c->status;
}

void exec_cmd(command_t cmd);
f_node_ extract_deps(command_t cmd);

f_node_
extract_simple_dep(command_t cmd)
{
  f_node_ file_node = NULL;
  if(cmd->input != NULL)
  {
    file_node = checked_malloc(sizeof(struct file_node));
    file_node->prev = NULL; file_node->next = NULL;
    file_node->fileName = cmd->input;
    file_node->type = READ_F;
  }
  if(cmd->output != NULL)
  {
    f_node_ out = NULL;
	if(file_node != NULL)
    {
      file_node->next = checked_malloc(sizeof(struct file_node));
      out = file_node->next; 
      out->prev = file_node; 
	  out->next = NULL;
    }
	else
    {
      file_node = checked_malloc(sizeof(struct file_node));
      file_node->prev = NULL; 
	  file_node->next = NULL;
      out = file_node;
    }
    out->type = WRITE_F;
    out->fileName = cmd->output;
  }
  int i = 1;
  if(file_node == NULL)
  {
    if(!cmd->u.word[i])
      return file_node;
    file_node = checked_malloc(sizeof(struct file_node));
    file_node->next = NULL; 
	file_node->prev = NULL;
    file_node->type = READ_F;
    file_node->fileName = cmd->u.word[i];
    i++;
  }
  f_node_ f_node2 = file_node;
  while(f_node2->next != NULL)
    f_node2 = f_node2->next;
  while(cmd->u.word[i])
  {
    f_node2->next = checked_malloc(sizeof(struct file_node));
    f_node2->next->prev = f_node2;
    f_node2 = f_node2->next;
    f_node2->fileName = cmd->u.word[i];
    f_node2->type = READ_F;
    f_node2->next = NULL;
  }
  return f_node2;
}

f_node_
extract_complex_dep(command_t cmd)
{
  f_node_ l = extract_deps(cmd->u.command[0]);
  f_node_ r = extract_deps(cmd->u.command[1]);
  if(r == NULL)
    return l;
  else if(l == NULL)
    return r;
  f_node_ left_node = l;
  f_node_ left_tail = left_node;
  while(left_node != NULL)
  {
    f_node_ right_node = r;
    while(right_node != NULL)
    {
      if(strcmp(right_node->fileName, left_node->fileName)== 0)
      {
        f_node_ tmp = right_node;
        if(right_node->prev != NULL && right_node->next != NULL)
        {
          right_node->prev->next = right_node->next;
          right_node->next->prev = right_node->prev;
          right_node = right_node->next;
        }
		else if(right_node->prev != NULL && right_node->next == NULL)
        {
          right_node->prev->next = NULL;
          right_node = NULL;
        }
        else if(right_node->prev == NULL && right_node->next != NULL)
        {
          right_node->next->prev = NULL;
          r = right_node = right_node->next;
        }
        else
        {
          r = NULL;
          right_node = NULL;
        }
        free(tmp);
      }
      else
        right_node = right_node->next;
    }
    left_tail = left_node;
    left_node = left_node->next;
  }
  if(r != NULL)
  {
    left_tail->next = r;
    r->prev = left_tail;
  }
  return l;
}

f_node_
extract_deps(command_t cmd)
{
  if(cmd->type == SIMPLE_COMMAND)
	   return extract_simple_dep(cmd);
  else if(cmd->type == SUBSHELL_COMMAND)
	   return extract_deps(cmd->u.subshell_command);
  else
	  return extract_complex_dep(cmd);
}
void 
exec_or_cmd(command_t cmd)
{
  exec_cmd(cmd->u.command[0]);
  if (cmd->u.command[0]->status != 0) 
  {
    exec_cmd(cmd->u.command[1]);
    cmd->status = cmd->u.command[1]->status;
  } 
  else 
  {
	cmd->status = cmd->u.command[0]->status;
  }
}

void 
exec_and_cmd(command_t cmd)
{
  exec_cmd(cmd->u.command[0]);
  
  if (cmd->u.command[0]->status != 0) { 
	   cmd->status = cmd->u.command[0]->status;
  } 
  else { 
    exec_cmd(cmd->u.command[1]);
    cmd->status = cmd->u.command[1]->status;  
  }
}

void
exec_pipe_cmd(command_t cmd)
{
  
  pid_t p;
  int arr[2];
  pipe(arr);

  if((p=fork()) == 0)
  {
    dup2(arr[0],0);
    close(arr[1]);
    exec_cmd(cmd->u.command[1]);
    close(arr[0]);
    exit(cmd->u.command[1]->status);
  } 
  else if(p > 0) 
  {
    pid_t p2;
    if((p2=fork()) == 0) {
      dup2(arr[1],1);
      close(arr[0]);
      exec_cmd(cmd->u.command[0]);
      close(arr[1]);
      exit(cmd->u.command[0]->status);
    }
	else if( p2 < 0)
		error(1, errno, "Forking Error."); 
	else 
	{
      int stat;
      close(arr[0]);
      close(arr[1]);
      
      pid_t wait_p = waitpid(-1,&stat,0);
      if(wait_p == p2)
      {
        waitpid(p,&stat,0);
        cmd->status = WEXITSTATUS(stat);
        return;
      }
      else if(wait_p == p2)
      {
		cmd->status = WEXITSTATUS(stat);;
        waitpid(p2,&stat,0);
        return;
      }
    } 
  } 
  else
  { 
	  error(1, errno, "Forking Error."); 
  }
}

void
exec_simple_cmd(command_t cmd)
{
  pid_t p = fork();
  if(p < 0)
	  error (1, 0, "Forking error.");
  else if(p > 0) {
    int stat;
    if(waitpid(p, &stat, 0) == -1)
      error(1, errno, "Child process exit error");
    cmd->status = WEXITSTATUS(stat);
  }
  else if(p == 0) {
    if(cmd->input != NULL)
    {
      int in = open(cmd->input, O_RDONLY);
      if(in == -1)
      error(1, errno, "Cannot open file as input");
      dup2(in, STDIN_FILENO);
      close(in);
    }
    if(cmd->output != NULL)
    {
      int out = open(cmd->output, O_WRONLY | O_CREAT, 0664);
      if(out == -1)
      error(1, errno, "Cannot open file as output");
      dup2(out, STDOUT_FILENO);
      close(out);
    }
    execvp(cmd->u.word[0], cmd->u.word);
    exit(cmd->status);
  } 
}

void exec_seq_cmd(command_t cmd)
{
  exec_cmd(cmd->u.command[0]);
  exec_cmd(cmd->u.command[1]);
  cmd->status = cmd->u.command[1]->status;
}

void exec_subshell_cmd(command_t cmd)
{
  exec_cmd(cmd->u.subshell_command);
  cmd->status = cmd->u.subshell_command->status;
}

void
exec_cmd(command_t cmd)
{
  if(cmd->type == OR_COMMAND)
    exec_or_cmd(cmd);
  else if(cmd->type == AND_COMMAND)
    exec_and_cmd(cmd);
  else if(cmd->type == PIPE_COMMAND)
    exec_pipe_cmd(cmd);
  else if(cmd->type == SIMPLE_COMMAND)
    exec_simple_cmd(cmd);
  else if(cmd->type == SEQUENCE_COMMAND)
    exec_seq_cmd(cmd);
  else if(cmd->type == SUBSHELL_COMMAND)
    exec_subshell_cmd(cmd);
  else
    error(1,0, "Not a valid command type. Error");
}

void
execute_command (command_t c, int time_travel)
{
  if(time_travel == 0)
    exec_cmd(c);
  ////////////////////////////////////////////////////////////
  //time travel not implemented for part b
	////////////////////////////////////////////////////////////
 else
	error(1, 0, "Error creating the thread"); 
}
