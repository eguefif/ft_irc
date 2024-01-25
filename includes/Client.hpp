#pragma once

#include <queue>
#include <string>

class Client
{
	public:
		Client(const std::string &pAddress);
		~Client();
		void addMsg(const std::string &msg);
		std::string getMsg();

	private:
		Client() {};
		Client	&operator=(const Client &other);
		Client(const Client &other);
		const std::string address;
		std::queue<std::string> outputQueue;
};
