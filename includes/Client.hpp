#pragma once

#include <queue>
#include <string>

#include "CommandGenerator.hpp"
#include "ACmd.hpp"

class CommandGenerator;

class Client
{
	public:
		Client(const std::string &pAddress, const int &pFd);
		~Client();
		void addMsg(const std::string &msg);
		std::string getMsg();

		ACmd *updateCmd(const std::string &message);

	private:
		Client();
		Client	&operator=(const Client &other);
		Client(const Client &other);

		const std::string address;
		std::queue<std::string> outputQueue;
		CommandGenerator *cmdGenerator;
};
