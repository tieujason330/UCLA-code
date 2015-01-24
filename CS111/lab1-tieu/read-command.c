// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <ctype.h>
#include <stdbool.h>
/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */
int lines; /*num of lines */
int (*get_byte) (void *);
void *get_byte_arg;

typedef struct command_node *command_node_t;

struct command_node
{
  command_t cmd;
  command_node_t next, prev;
};

struct command_stream
{
  command_node_t* commands;
};

command_t create_subshell_cmd(char *buf);
command_t create_cmd(char *buf, enum command_type t);

void
syntax_error_message()
{
  error(1, 0, "Syntax error at line %d", lines);
}

void 
remove_whitespaces()
{
  if(!feof(get_byte_arg))
  {
    char x = get_byte(get_byte_arg);
    while(strchr("\n\t ", x))
    {
      if(x == '\n') lines++;
      x = get_byte(get_byte_arg);
    }
    ungetc(x, get_byte_arg);
  }
}

enum command_type
get_cmd_type(char *buf)
{
  while(!feof(get_byte_arg))
  { 
    char x = get_byte(get_byte_arg);
    char y = get_byte(get_byte_arg);
    if(!strchr("\n\t ", x) && y == '#')
      syntax_error_message();
    ungetc(y, get_byte_arg);
    switch(x)
    {
	  case '(':
      {
        remove_whitespaces();
        return SUBSHELL_COMMAND;
      }
      case ')':
      {
        ungetc(x, get_byte_arg);
        return SIMPLE_COMMAND;
      }
      case '#':
        y = get_byte(get_byte_arg);
      	while(y != '\n')
      	{
            if(y == EOF) return SIMPLE_COMMAND;
        	  y = get_byte(get_byte_arg);
      	}
        return get_cmd_type(buf);
      case '&':
        y = get_byte(get_byte_arg);
        if(y == '&')
        {
          remove_whitespaces();
          return AND_COMMAND;
        }
        else if(y == EOF)
          syntax_error_message();
        else 
          ungetc(y, get_byte_arg);
        break;
	  case '|':
        y = get_byte(get_byte_arg);
        if(y == '|')
        {
          remove_whitespaces();
          return OR_COMMAND;
        }
        else if(isalnum(y) || strchr("_^@:/.-,+%!\t\n ", y))
        {
          ungetc(y, get_byte_arg);
          remove_whitespaces();
          return PIPE_COMMAND;
        }
        else if(y == EOF)
          syntax_error_message();
      
      case '\n': lines++;
      case ';': case EOF:
        return SIMPLE_COMMAND;
    }
    buf[strlen(buf)] = x;
  }
  return SEQUENCE_COMMAND;
}

command_t
create_simple_cmd(char *buf)
{
  if(!strlen(buf))
    syntax_error_message();
  command_t cmd = checked_malloc(sizeof(struct command));
  cmd->status = -1;
  cmd->type = SIMPLE_COMMAND; 
  cmd->input = NULL; 
  cmd->output = NULL;
  cmd->u.word = checked_malloc(8*sizeof(char*)); 

  bool is_input = false; 
  bool is_output = false;
  bool inside_in = false; 
  bool inside_out = false;
  bool inside_word = false;

  size_t size_of_in = 8;
  size_t size_of_out = 8;      
  size_t size_of_word = 8;
  size_t cur_size_of_word; 
  size_t idx = 0; 
   
  int x;
  for(x = 0; buf[x]; x++)
  {
    if(strchr("_^@:/.-,+%!", buf[x]) || isalnum(buf[x]))
    {
      if(inside_in)
      {
        is_input = true;
        char* str = cmd->input;
        if(strlen(str) >= size_of_in) checked_grow_alloc(str, &size_of_in);
        str[strlen(str)] = buf[x];
      }
      else if(inside_out)
      {
        is_output = true;
        char* str = cmd->output;
        if(strlen(str) >= size_of_out) checked_grow_alloc(str, &size_of_out);
        str[strlen(str)] = buf[x];
      }
	  else if(inside_word)
      {
        char *str = cmd->u.word[idx];
        if(strlen(str) >= cur_size_of_word) checked_grow_alloc(str, &cur_size_of_word);
        str[strlen(str)] = buf[x];
      }
      else if(!inside_word)
      {
        if((is_input || is_output) && !inside_in && !inside_out)
          syntax_error_message();
        if(idx >= size_of_word) checked_grow_alloc(cmd->u.word, &size_of_word);
        cmd->u.word[idx] = checked_malloc(8*sizeof(char));
        cur_size_of_word = 8;
        cmd->u.word[idx][0] = buf[x];
        inside_word = true;
      }
    }
	else if(buf[x] == '<')
    {
      if(x == 0 || inside_in || inside_out || is_input || is_output )
        syntax_error_message();
      inside_in = true;
      cmd->input = checked_malloc(8*sizeof(char));
    }
    else if(buf[x] == '>')
    {
      if(x == 0 || is_output || inside_out) syntax_error_message();
      cmd->output = checked_malloc(8*sizeof(char)); 
      inside_in = false; inside_out = true;
    }
	else if(buf[x] == EOF)
    {
      if(idx >= size_of_word) checked_grow_alloc(cmd->u.word, &size_of_word);
      return cmd;
    }
    else if(strchr("\t ", buf[x]))
    {
      if(inside_word)
      {
        inside_word = false;
        idx++;
      }
      else if(is_output && inside_out) inside_out = false; 
      else if(is_input && inside_in) inside_in = false;
    }
    else
      syntax_error_message();
  }
  memset((void *) buf, '\0', 1024);
  if(idx >= size_of_word)
        checked_grow_alloc(cmd->u.word, &size_of_word);
  return cmd;
}

command_t
create_compound_cmd(char *buf, enum command_type type, command_t caller)
{
  command_t compound_cmd = checked_malloc(sizeof(struct command));
  compound_cmd->status = -1;
  compound_cmd->type = type; 
  if(caller == NULL)
    compound_cmd->u.command[0] = create_simple_cmd(buf);
  else if(caller->type == SUBSHELL_COMMAND || (type != PIPE_COMMAND && caller->type == PIPE_COMMAND) || (type == PIPE_COMMAND) == (caller->type == PIPE_COMMAND))
    compound_cmd->u.command[0] = caller;
  else if(type == PIPE_COMMAND && caller->type != PIPE_COMMAND)
    compound_cmd->u.command[0] = caller->u.command[1];
  enum command_type adj_type = get_cmd_type(buf);
  if(adj_type == SIMPLE_COMMAND || adj_type == SEQUENCE_COMMAND)
  {
    compound_cmd->u.command[1] = create_simple_cmd(buf);
    return compound_cmd;
  }
  else if(adj_type == SUBSHELL_COMMAND)
  {
    command_t sub = create_subshell_cmd(buf);
    adj_type = get_cmd_type(buf);
    if(adj_type == SIMPLE_COMMAND)
    {
      compound_cmd->u.command[1] = sub;
      return compound_cmd;
    }
    else if(type != PIPE_COMMAND && adj_type == PIPE_COMMAND)
    {
      compound_cmd->u.command[1] = sub;
      compound_cmd->u.command[1] = create_compound_cmd(buf, adj_type, compound_cmd);
      return compound_cmd;
    }
    else
    {
      compound_cmd->u.command[1] = sub;
      command_t adj_cmd = create_compound_cmd(buf, adj_type, compound_cmd);
      return adj_cmd;
    }
  }
  else if(type != PIPE_COMMAND && adj_type == PIPE_COMMAND)
  {
    compound_cmd->u.command[1] = create_simple_cmd(buf);
    compound_cmd->u.command[1] = create_compound_cmd(buf, adj_type, compound_cmd);
    return compound_cmd;
  }
  else
  {
    compound_cmd->u.command[1] = create_simple_cmd(buf);
    command_t adj_cmd = create_compound_cmd(buf, adj_type, compound_cmd);
    return adj_cmd;
  }
}

command_t
create_subshell_cmd(char *buf)
{
  command_t sub = checked_malloc(sizeof(struct command));
  sub->type = SUBSHELL_COMMAND; sub->status = -1;
  enum command_type type = get_cmd_type(buf);
  command_t cmd = create_cmd(buf, type);
  remove_whitespaces();
  char x;
  if((x = get_byte(get_byte_arg)) == ')')
  {
    sub->u.subshell_command = cmd;
    return sub;
  }
  else
  {
    ungetc(x, get_byte_arg);
    command_t t = checked_malloc(sizeof(struct command)); 
    t->type = SEQUENCE_COMMAND; 
	t->status = -1;
    t->u.command[0] = cmd; 
	t->u.command[1] = NULL;
    while((x = get_byte(get_byte_arg)) != ')')
    {
      ungetc(x,get_byte_arg);
      enum command_type type = get_cmd_type(buf);

      command_t new_seq = checked_malloc(sizeof(struct command));
      new_seq->type = SEQUENCE_COMMAND; 
	  new_seq->status = -1;
      new_seq->u.command[0] = create_cmd(buf, type);
      new_seq->u.command[1] = NULL;
      command_t b = t;
      while(b->u.command[1] != NULL)
        b = b->u.command[1];
      b->u.command[1] = new_seq;
    }
    command_t b = t;
    while(b->u.command[1]->u.command[1] != NULL)
      b = b->u.command[1];
    b->u.command[1] = b->u.command[1]->u.command[0];
    sub->u.subshell_command = t;
    return sub;
  }
}

command_t
create_cmd(char* buf, enum command_type type)
{
  if(type == SUBSHELL_COMMAND)
  {
    command_t sub = create_subshell_cmd(buf);
    type = get_cmd_type(buf);
    if(type == SIMPLE_COMMAND)
      return sub;
    else
      return create_compound_cmd(buf, type, sub);
  }
  else if(type == SIMPLE_COMMAND)
    return create_simple_cmd(buf);
  else
    return create_compound_cmd(buf, type, NULL);
}

command_node_t
create_cmd_node(char* buf, enum command_type type)
{
  command_node_t node = checked_malloc(sizeof(struct  command_node));
  node->cmd = create_cmd(buf,type);
  node->next = NULL;
  return node;
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  get_byte = get_next_byte;
  get_byte_arg = get_next_byte_argument;
  lines = 1;
  char buf[1024] = "";
  command_stream_t cmd_stream = checked_malloc(sizeof(struct command_stream));
  command_node_t cmd_node = checked_malloc(sizeof(struct command_node));
  command_node_t node_head = NULL;    
  command_node_t node_tail = NULL;
  command_node_t tmp_node = cmd_node;

  if(!feof(get_byte_arg))
  {
    remove_whitespaces();
    if((buf[0] = get_byte(get_byte_arg)) == EOF)
    {
      free(cmd_stream);
      free(cmd_node);
      return NULL;
    }
    ungetc(buf[0], get_byte_arg);
    buf[0] = '\0';
    enum command_type type = get_cmd_type(buf);  

    while(1)  
    {
      if(type == SEQUENCE_COMMAND)
        tmp_node = create_cmd_node(buf, SIMPLE_COMMAND);
      else
        tmp_node = create_cmd_node(buf, type);	
      if(node_head)
      {
		node_tail->next = tmp_node;
        tmp_node->prev = node_tail;
      }
      else
      {
	    node_head = tmp_node;
      }
      node_tail = tmp_node;
      if(type == SEQUENCE_COMMAND)
        break;
      remove_whitespaces();
      if((buf[0] = get_byte(get_byte_arg)) == EOF)
      {
       cmd_stream->commands = &node_head;
        return cmd_stream;
      }
      ungetc(buf[0], get_byte_arg);
      buf[0] = '\0';
      type = get_cmd_type(buf);
    }
  }
 cmd_stream->commands = &node_head;
  return cmd_stream;
}

command_t
read_command_stream (command_stream_t s)
{
  if(s == NULL)
    return NULL;
  if(*(s->commands) != NULL)
  {
    command_node_t cmd_stream = *(s->commands);
    *(s->commands) =cmd_stream->next;
    if(cmd_stream->prev != NULL)
    {
      free(cmd_stream->prev->cmd);
      free(cmd_stream->prev);
    }
    return cmd_stream->cmd;
  }
  return NULL;
}