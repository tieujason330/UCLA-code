//
//  main.cpp
//  client
//
//  Created by I-Chun Liu and Jason Tieu on 11/22/14.
//  Copyright (c) 2014 I-Chun LIU. All rights reserved.
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>      // define structures like hostent
#include <stdlib.h>
#include <strings.h>

#include <string>
#include <cstring>
#include <iostream>
#include <ctime>

#define PACKETSIZE 1000

struct packet {
	int type; //0 - req, 1 - data, 2 - ACK, 3 - fIN
	int seq;  //pckt sequence num
	int size; //data size
	char data[PACKETSIZE];
};

//this function prints out packet info and distinguishes between received or sent
void printPacket(struct packet p, int io, unsigned start) {

	//std::cout<< "start " << start << std::endl;

	unsigned elap = time(0) - start; //elapsed time

	std::cout<< "elap " << elap << std::endl;
	if (io == 0) 
		std::cout << "RECV: Type " << p.type << " Seq " << p.seq << " Size " << p.size;
	else
		std::cout << "SENT: Type " << p.type << " Seq " << p.seq << " Size " << p.size;
	std::cout << "  **TimeStamp:" << elap << "ms" << std::endl;
}

//simulates loss and corruption
int simulateFault(double prob) {
    double randomProb = (double)rand() / (double)RAND_MAX;
    if(randomProb <= (double) prob){
        std::cout << "Random Probability:" << randomProb;
        std::cout << ". Assigned probability by user:" << prob;
        std::cout << ".\n";
        return 1;
    }
    std::cout << "Random Probability:" << randomProb;
    std::cout << ". Assigned probability by user:" << prob;
    std::cout << ".\n";
	return 0;
}

int main(int argc, char * argv[]) {
	int sockfd; //Socket descriptor
	int portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server; //contains tons of information, including the server's IP address
	int current;
	double prob_loss, prob_corrupt;
	struct packet p_in, p_out;
	FILE *f;

	unsigned start_t = time(0); //for timestamp
	//changed to 6 arguments, including prob_loss and prob_corrupt
	if (argc < 6 || argc > 6) {
		fprintf(stderr,"Format: ./client <sender_hostname> <sender_portnumber> <filename> <prob_loss> <prob_corrupt>\n", argv[0]);
		exit(0);
	}
	
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); //create a new socket
	if (sockfd < 0){
		perror("ERROR, opening socket");
		exit(1);
	}

	
	server = gethostbyname(argv[1]); //takes a string like "www.yahoo.com", and returns a
									 //struct hostent which contains information, as IP
									 //address, address type, the length of the addresses...
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	
	//get prob_loss and prob_corrupt
	prob_loss = atof(argv[4]);
	prob_corrupt = atof(argv[5]);
	
	//make sure loss/corrupt within 0.0 to 1.0
	if(prob_loss < 0.0 || prob_loss > 1.0 || prob_corrupt < 0.0 || prob_corrupt > 1.0) {
		perror("ERROR prob must be between 0.0 and 1.0");
		exit(0);
	}

	//Setting up serv_addr
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; //initialize server's address
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	
	//Get the file name from argv[3]
	//changed from string to char *
	char *fileName = argv[3];
	
	//need to build request
	std::cout << "Building request for " << fileName << ".\n";
	bzero((char *) &p_out, sizeof(p_out));
	p_out.type = 0; //request
	p_out.seq = 0;
	p_out.size = strlen(fileName) + 1;
	strcpy(p_out.data, fileName);

	//send req
	if(sendto(sockfd, &p_out, sizeof(p_out), 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR sending request.\n");
		exit(0);	
	}	
	printPacket(p_out, 1, start_t);
	std::cout << "--------------------------------\n";

    //Expected sequence number from server
	current = 0;
	f = fopen(strcat(fileName, "_copy"), "wb");
	
	//ACK template
	bzero((char *)&p_out, sizeof(p_out));
	p_out.type = 2; //ACK
	p_out.seq = current - 1;
	p_out.size = 0;

	srand(time(NULL));
	socklen_t serv_length = sizeof(serv_addr);
	//get responses
	while(1) {
		if(recvfrom(sockfd, &p_in, sizeof(p_in), 0, (struct sockaddr*) &serv_addr, &serv_length) < 0)
			std::cout << "Packet lost.\n";
		else {
			//simulate corrupt/loss
			if(simulateFault(prob_loss)) {
				std::cout << "Simulated loss " << p_in.seq << ".\n";
				continue;
			}
			else if(simulateFault(prob_corrupt)) {
				std::cout << "Simulated corruption " << p_in.seq << ".\n";
				//resend previous ACK
				if(sendto(sockfd, &p_out, sizeof(p_out), 0, (struct sockaddr*) &serv_addr
					, sizeof(serv_addr)) < 0) {
					perror("ERROR resending ACK.\n");
					exit(0);
				}
				printPacket(p_out, 1, start_t);
				continue;
			}
			printPacket(p_in, 0, start_t);
            
			//determine pckt num and type
			if(p_in.seq < current) {
				std::cout << "IGNORE " << p_in.seq << ": expected " << current << ".\n";
				p_out.seq = p_in.seq; // send ACK regardless
			}
			else if(p_in.seq > current) {
				std::cout << "IGNORE " << p_in.seq << ": expected " << current << ".\n";
				continue;
			}
			else {
				if(p_in.type == 3) 
					break; //FIN received
				else if(p_in.type != 1) {
					std::cout << "IGNORE " << p_in.seq << ": not a data packet.\n";
					continue;
				}

				//should only write for expected data packet
				fwrite(p_in.data, 1, p_in.size, f);
				p_out.seq = current;
				current++;
			}
		
			if(sendto(sockfd, &p_out, sizeof(p_out), 0, (struct sockaddr*) &serv_addr, 
				sizeof(serv_addr)) < 0) {
				perror("ERROR sending ACK.\n");
				exit(0);
			}
			printPacket(p_out, 1, start_t);
		}	
	/*
	//Sends a message to the sender which includes the name of the file requested
	if(sendto(sockfd, fileName.c_str(), fileName.length(), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		std::cout << "Message(file name) DID NOT send out correctly\n";
		return 0;
	*/
	}
	
	//sending FIN packet
	bzero((char *) &p_out, sizeof(p_out));
	p_out.type = 3; //FIN 
	p_out.seq = current;
	p_out.size = 0;
	if(sendto(sockfd, &p_out, sizeof(p_out), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR sending FIN packet.\n");
		exit(0);
	}
	printPacket(p_out, 1, start_t);

	std::cout << "Exiting.\n";
	fclose(f);
	//close(sockfd);
	
    return 0;
}
