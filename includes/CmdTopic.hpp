#pragma once

#include "Client.hpp"
#include "ACmd.hpp"

class Client;

class CmdTopic: public ACmd
{
	public:
		~CmdTopic() {};
		CmdTopic(const int &pFd, const std::string &pMessage);

		void execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);
	
	private:
		CmdTopic();
		CmdTopic(const CmdTopic &other);
		CmdTopic &operator=(const CmdTopic &other);
		std::string checkError(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);
		bool checkUserInChan(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList);
		bool checkUserIsOp(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList);
};
