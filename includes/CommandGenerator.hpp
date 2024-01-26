#pragma once

#include <sstream>
#include "Client.hpp"
#include "CmdNick.hpp"

#define CMD_LIST_NUMBER 2

class Client;
class ACmd;

class CommandGenerator
{
	public:
		CommandGenerator();
		~CommandGenerator();

		ACmd *update(const std::string &msg);
		void setFd(const int &pFd);

	private:
		CommandGenerator(const CommandGenerator &other);
		CommandGenerator &operator=(const CommandGenerator &other);

		ACmd *generate();
		int getIndex(const std::string &cmd);

		int senderFd;
		std::string msg;

		std::string commandList[CMD_LIST_NUMBER];
};
