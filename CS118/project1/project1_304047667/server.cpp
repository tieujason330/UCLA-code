#include "server.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <linux/limits.h>
#include <sys/wait.h>

#define MAXIMUM_CONNECTIONS 128
/* Constructor to instantiate server 
*/
Server::Server(int port_num, std::string root)
{
	m_port_num = port_num;
	m_web_root = root;
	/* need to append "/" to web_root if it doesn't have it */
	int end = m_web_root.length()-1;
	if(m_web_root[end] != '/') m_web_root += "/";

	std::cout << "Server starting with root: ";
	/* print current working directory for relative path */
	if(m_web_root[0] != '/')
	{
		char buffer[PATH_MAX]; //defined as 4096 in limits.h
		if(getcwd(buffer, sizeof(buffer))) std::cout << buffer << "/";
	}
	std::cout << m_web_root << std::endl;
}

/* Destructor ends all child forks when server exits 
*/
Server::~Server()
{
	end_child_forks(SIGINT); //signal.h
}

/* End all child forks when called */
void Server::end_child_forks(int signal)
{

	Child_Fork::iterator i = m_child_forks.begin();;

	while( i != m_child_forks.end())
	{
		kill(*i, signal);
		i++;
	}

	waitpid(-1,NULL,0);
	m_child_forks.clear();
}

/* Server must listen for a connection 
*/
void Server::listen()
{
	m_socket.create_soc();
	m_socket.bind_soc(m_port_num);
	int listen_port_num = m_socket.sock_port_number();
	bool loop = true;

	//make sure socket port number is correct
	if(listen_port_num != m_port_num)
	{
		std::cout << " ----WARNING---- Cannot bind to port number: " << m_port_num << std::endl; 
	}
	else if(listen_port_num == 0 || listen_port_num == -1)
	{
		std::cout << " ----ERROR---- Ending, cannot open socket on port number: " << m_port_num << "." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << " **** Listening on port number: " << listen_port_num << std::endl;
	m_socket.listen(MAXIMUM_CONNECTIONS);

	while(loop)
	{
		Socket new_socket;
		m_socket.accept(new_socket);

		pid_t p = fork(); //child handles request
		bool is_child = false;
		
		if(p == 0) {is_child = true;}
		else if(p > 0)
		{
			m_child_forks.insert(p);
			continue;
		}

		if(is_child) m_child_forks.clear(); //clear child pids from proc
		
		std::string res;
		new_socket.read_data(res);
		send_request_response(new_socket, res);

		if(is_child) exit(0); //kill child
	}
}

/* Must send a response to request back to browser by assembling HTTP response with headers/content.
*/
void Server::send_request_response(Socket& resp_soc, const std::string& req_data)
{
	std::string file_name = "", file_exten ="", response_code = "", mime_type = "";
	std::stringstream header;
	std::ifstream filestream;
	std::string resp_data;

	/* Parse HTTP header to figure out what file we want (first line)
	*/
	char* req_cpy = new char[req_data.size() + 1];
	memset(req_cpy, '\0', req_data.size() + 1);
	strncpy(req_cpy, req_data.c_str(), req_data.size());

	char* tok = strtok(req_cpy, " " ); //GET or POST
	tok = strtok(NULL, " "); //URL
	std::string f_name = "";
	
	if(tok != NULL) file_name = tok;

	delete req_cpy;

	if(file_name[0] == '/') file_name = file_name.substr(1, std::string::npos); //remove '/'
	else if(file_name == "/") file_name = "test.html";

	size_t extension_idx = file_name.rfind('.');
	/* string::npos = means not found */
	if(std::string::npos != extension_idx) file_exten = file_name.substr(extension_idx, std::string::npos);

	//get mime_type
	transform(file_exten.begin(), file_exten.end(), file_exten.begin(), ::tolower);
	
	if(file_exten == ".jpeg" || file_exten == ".jpg") mime_type = "image/jpeg";
	else if(file_exten == ".html") mime_type = "text/html; charset=utf-8";
	else if(file_exten == ".gif") mime_type = "image/gif";
	else mime_type = "test/plain";

	//is file_name is valid, open file
	if(file_name.size() > 0) 
	{
		file_name = m_web_root + file_name; //appends relative path
		filestream.open(file_name.c_str(), std::ifstream::in);	
	}	

	if(filestream.is_open()) response_code = "200 OK";
	else response_code = "404 NOT FOUND";

	char x = filestream.get();
	while(filestream.good())
	{
		resp_data.push_back(x);
		x = filestream.get();
	}
	filestream.close();

	//response header
	header << "HTTP/1.1 " << response_code << std::endl;
	if(response_code == "404 NOT FOUND")
	{
		header << "Content-Length: " << response_code.size() << std::endl << std::endl;
		resp_soc.write_data(header.str());
		resp_soc.write_data(response_code);
	
		std::cout << "---- Client Request ----" << std::endl << std::endl <<req_data << std::endl;
		
		//parses HTTP request from browser
		//show response that would be sent to client
		std::cout << std::endl << header.str() << std::endl;		
		return;
	}
	
	header << "Content-Type: " << mime_type << std::endl;
	header << "Content-Length: " << resp_data.size() << std::endl << std::endl;
	resp_soc.write_data(header.str());
	resp_soc.write_data(resp_data);
	std::cout << " ---- Client Request ----" << std::endl << std::endl << req_data << std::endl;

	//parses HTTP request from browser
	//show response that would be sent to client
	std::cout << std::endl << header.str() << std::endl;
}





































