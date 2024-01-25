#pragma once

#include <sstream>
#include "Client.hpp"

#define CMD_LIST_NUMBER 2

public CommandGenerator
{
	public:
		CommandGenerator(const int &pSenderFd, std::map<int, Client*> &pClientList);
		virtual ~CommandGenerator();

		ACmd *update(const std::string &msg);

	private:
		CommandGenerator() {};
		CommandGenerator(const CommandGenerator &other);
		CommandGenerator &operator=(const CommandGenerator &other);

		ACmd *generate();
		int getIndex(const std::string &cmd);

		int senderFd;
		std::string msg;
		std::map<int, Client*> &clientList;

		const static std::string commandList[] = {"NICK", "USER"};
};
