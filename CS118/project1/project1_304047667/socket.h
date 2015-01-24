#ifndef SOCKET
#define SOCKET

#include <unistd.h>
#include <string>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_REQUEST_SIZE 8192 /* FF (8 kb) is the largest data bits can be */

const int SOCKET_DEFAULT = -1;

class Socket
{
public:
	Socket();
	virtual ~Socket();
	int create_soc();
	int bind_soc(int port_num);
	int listen(int backlog = 1);
	int accept(Socket& new_socket);
	int write_data(std::string d);
	int read_data(std::string& d);
	int sock_port_number();
private:
	int m_sockfd;
	sockaddr_in m_sock_addr;
};

#endif
