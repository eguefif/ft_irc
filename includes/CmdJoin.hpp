#pragma once

#include "defines.hpp"
#include "Client.hpp"
#include <iostream>
#include <string>
#include "ACmd.hpp"

class Client;

class CmdJoin : public ACmd
{
	public:
		~CmdJoin() {};
		CmdJoin(const int &pFd, const std::string pMessage);

		void execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);
	
	private:
		CmdJoin();
		CmdJoin(const CmdJoin &other);
		CmdJoin &operator=(const CmdJoin &other);
		bool checkChannelUnicity(std::map<std::string, Channel *> &channelList);
		std::string checkError(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);

		std::vector<std::string> channels;
		std::vector<std::string> keys;


		const std::string getNewChannelName();
};
