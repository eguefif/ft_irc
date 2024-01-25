#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <map>

#include "../includes/Log.hpp"
#include "../includes/Client.hpp"

#define SERV_MAX_CLIENTS 100

class Server
{
	public:
		Server() {};
		~Server();

		Server(const std::string &pPort, const std::string &pPass);

		void run();
	
	private:
		std::map<int, Client*> clientList;
		int port;
		std::string pass;
		int serverSocket;
		nfds_t numSockets;
		struct sockaddr_in addressServer;
		struct pollfd *pfds;

		Server(const Server &other);
		Server &operator=(const Server &other);

		int initConnection(struct sockaddr_in &address);
		void newConnection();
		void initPoll();
};
