#pragma once

#include "defines.hpp"
#include "Client.hpp"
#include <iostream>
#include "ACmd.hpp"

class Client;

class CmdQuit: public ACmd
{
	public:
		~CmdQuit() {};
		CmdQuit(const int &pFd, const std::string &pMessage);

		void execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);
	
	private:
		CmdQuit();
		CmdQuit(const CmdQuit &other);
		CmdQuit &operator=(const CmdQuit &other);
		std::string checkError(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);

		std::string getQuitMsg(const std::string &nickname);
		std::string getQuitMsgBroadcast(const std::string &nickname);
};
