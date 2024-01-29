#pragma once

#include <queue>
#include <string>

#include "Log.hpp"

#define SEP "\n"
#define MAX_MSG_SIZE 512

class Client
{
	public:
		Client(const std::string &pAddress);
		~Client();
		void addMsg(const std::string &msg);
		std::string getMsg();

		void updateMsg(const std::string &message);
		void setNickname(const std::string &nickname);
		std::string getNextMessage();

	private:
		Client();
		Client	&operator=(const Client &other);
		Client(const Client &other);

		std::string	inputMsg;
		std::string nickname;
		const std::string address;
		std::queue<std::string> outputQueue;
};
