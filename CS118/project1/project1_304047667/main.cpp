#include "server.h"
#include "socket.h"

#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

using namespace std;

#define DEFAULT_PORT_NUMBER 5992

Server *server = NULL;

/* function to terminate server (CTRL+C) */
void signal_handler(int signal)
{
	//printf("exiting\n");
	
	//printf("BEFORE\n");
	server->end_child_forks(signal);
	delete server;
	server = NULL;
	//printf("AFTER\n");
	exit(signal);
}

int main(int argc, char **argv)
{
	signal(SIGHUP, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	std::string def_root = "./test_files/"; //default root folder 
	int port_num = DEFAULT_PORT_NUMBER; //using default port number 5992

	server = new Server(port_num, def_root);
	server->listen();

	return EXIT_SUCCESS;
}
