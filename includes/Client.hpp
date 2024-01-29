#pragma once

#include <queue>
#include <string>

#include "defines.hpp"
#include "Log.hpp"


class Client
{
	public:
		Client(const std::string &pAddress);
		~Client();
		void addMsg(const std::string &msg);
		std::string getMsg();

		void updateMsg(const std::string &message);
		std::string getNickname();
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
