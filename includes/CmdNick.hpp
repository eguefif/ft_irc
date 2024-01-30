#pragma once

#include "defines.hpp"
#include "Client.hpp"
#include <iostream>
#include "ACmd.hpp"

class Client;

class CmdNick : public ACmd
{
	public:
		~CmdNick() {};
		CmdNick(const int &pFd, const std::string pMessage);

		void execute(std::map<int, Client *> &clientList);
		std::string checkError(std::map<int, Client *> &clientList);
	
	private:
		CmdNick();
		CmdNick(const CmdNick &other);
		CmdNick &operator=(const CmdNick &other);
		bool checkNickUnicity(std::map<int, Client *> &clientList);

		const std::string getNewNickname();
};
