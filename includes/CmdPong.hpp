#pragma once

#include "defines.hpp"
#include "Client.hpp"
#include <iostream>
#include "ACmd.hpp"

class Client;

class CmdPong: public ACmd
{
	public:
		~CmdPong() {};
		CmdPong(const int &pFd, const std::string &pMessage);

		void execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);
	
	private:
		CmdPong();
		CmdPong(const CmdPong &other);
		CmdPong &operator=(const CmdPong &other);
		std::string checkError(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);

};
