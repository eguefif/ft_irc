#pragma once

#include <queue>
#include <string>

#include "Log.hpp"

class Client
{
	public:
		Client(const std::string &pAddress);
		~Client();
		void addMsg(const std::string &msg);
		std::string getMsg();

		std::string updateCmd(const std::string &message);
		void setNickname(const std::string &nickname);

	private:
		Client();
		Client	&operator=(const Client &other);
		Client(const Client &other);

		std::string	currCmd;
		std::string nickname;
		const std::string address;
		std::queue<std::string> outputQueue;
};
