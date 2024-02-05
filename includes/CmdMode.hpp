#pragma once

#include "defines.hpp"
#include "Client.hpp"
#include <iostream>
#include "ACmd.hpp"
#include <queue>

class Client;

class CmdMode: public ACmd
{
	public:
		~CmdMode() {};
		CmdMode(const int &pFd, const std::string &pMessage);

		void execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);
	
	private:
		CmdMode();
		CmdMode(const CmdMode &other);
		CmdMode &operator=(const CmdMode &other);
		std::string checkError(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);

		std::string replyMsg;
		std::string replyParams;

		void addFlags(std::string str);
		std::string handleFlag(std::string str, Channel *currentChannel);
		std::string getNextArg();
		std::string getBroadcastMsg(std::string nickname);
		std::string getChannelMode(std::map<std::string, Channel *> channelList);
		bool checkUserInChan(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList);
		
		std::string channel;
		std::vector<std::string> flags;
		std::queue<std::string> args;
};
