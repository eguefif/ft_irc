#pragma once

#include "Client.hpp"
#include "ACmd.hpp"

class Client;

class CmdUser: public ACmd
{
	public:
		~CmdUser() {};
		CmdUser(const int &pFd, const std::string &pMessage);

		void execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);
	
	private:
		CmdUser();
		CmdUser(const CmdUser &other);
		CmdUser &operator=(const CmdUser &other);
		std::string checkError(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);
};
