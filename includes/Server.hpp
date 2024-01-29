#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <map>

#include "Client.hpp"
#include "ACmd.hpp"
#include "Log.hpp"

#define HOST "0.0.0.0"
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
		int port;
		std::string pass;
		int serverSocket;
		nfds_t numSockets;
		struct sockaddr_in addressServer;
		std::vector<pollfd> pfds;
		std::vector<int> closedPfdsIndex;

		Server(const Server &other);
		Server &operator=(const Server &other);

		void setPort(const std::string &pPort);
		void initServerSocket();
		void createServerSocket();
		void createSocket();
		void setupSocket();
		void setNonBlockingSocket(const int &fd);
		void runBind();
		void runListen();
		void setAddressServerStruct();
		void initPoll();
		void runPoll();
		void handleNewconnection();
		void newConnection();
		void handlePauline();
		std::string readSocket(const int &fd);
		bool isReadError(const int &retVal) const;
		void listenPauline();
		void removeClosedConnections();
		bool isConnectionClosed(const int &retVal) const;
		void runCommands();
		void handlePollout();
		void removeClient(const int &fd);
};
