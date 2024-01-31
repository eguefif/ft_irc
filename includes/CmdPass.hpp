#pragma once

#include "defines.hpp"
#include "Client.hpp"
#include <iostream>
#include "ACmd.hpp"

class Client;

class CmdPass: public ACmd
{
	public:
		~CmdPass() {};
		CmdPass(const int &pFd, const std::string &pMessage, const std::string &pPass);

		void execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);
	
	private:
		CmdPass();
		CmdPass(const CmdPass &other);
		CmdPass &operator=(const CmdPass &other);
		std::string checkError(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);

		const std::string &password;

};
