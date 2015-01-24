#ifndef SERVER
#define SERVER

#include "socket.h"
#include <set>
#include <string>

class Server
{
public:
	typedef std::set<pid_t> Child_Fork;
	Server(int port_num, std::string root);
	~Server();
	void end_child_forks(int signal);
	void listen();
	void send_request_response(Socket& resp_soc, const std::string& req_data);

private:
	int m_port_num;
	Child_Fork m_child_forks;
	std::string m_web_root;
	Socket m_socket;
};

#endif 
