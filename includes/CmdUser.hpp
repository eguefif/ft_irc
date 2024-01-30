#pragma once

#include "defines.hpp"
#include "Client.hpp"
#include <iostream>
#include "ACmd.hpp"

class Client;

class CmdUser: public ACmd
{
	public:
		~CmdUser() {};
		CmdUser(const int &pFd, const std::string &pMessage);

		void execute(std::map<int, Client *> &clientList);
		std::string checkError(std::map<int, Client *> &clientList);
	
	private:
		CmdUser();
		CmdUser(const CmdUser &other);
		CmdUser &operator=(const CmdUser &other);
};
