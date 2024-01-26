#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <iostream>
#include <vector>
#include <map>

#include "Client.hpp"
#include "ACmd.hpp"
#include "Log.hpp"

#define SERV_MAX_CLIENTS 100
#define MAX_MSG_SIZE 512

ACmd *cmdFactory(std::string cmd, int senderFd);
class Server
{
	public:
		Server() {};
		~Server();

		Server(const std::string &pPort, const std::string &pPass);

		void run();
	
	private:
		std::map<int, Client*> clientList;
		std::vector<ACmd *> cmdList;
		int port;
		std::string pass;
		int serverSocket;
		nfds_t numSockets;
		struct sockaddr_in addressServer;
		std::vector<pollfd> pfds;
		std::vector<int> closedPfdsIndex;

		Server(const Server &other);
		Server &operator=(const Server &other);

		int initConnection(struct sockaddr_in &address);
		void newConnection();
		void initPoll();
		void removeClient(const int &fd, int index);
};

void setNonBlockingSocket(const int &fd);
