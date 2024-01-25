#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "Log.hpp"

class Server
{
	public:
		Server() {};
		~Server() {};

		Server(const std::string &pPort, const std::string &pPass);

		void run();
	
	private:
		int port;
		std::string pass;
		int serverSocket;
		struct sockaddr_in addressServer;

		int initConnection(struct sockaddr_in &address);
		Server(const Server &other);
		Server &operator=(const Server &other);
};
