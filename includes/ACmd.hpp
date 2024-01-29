#pragma once

#include <sstream>
#include <map>
#include <vector>
#include "stringUtils.hpp"
#include <exception>

#include "Client.hpp"

class Client;

class ACmd
{
	public:
		ACmd(const int &pFd, const std::string &pMessage);
		ACmd(const ACmd &other); 
		ACmd &operator=(const ACmd &other);
		virtual ~ACmd() {};

		virtual void execute(std::map<int, Client *> &clientList) = 0;
	
	protected:
		const int fd;
		std::string prefix;
		std::string command;
		std::vector<std::string> params;

	private:
		ACmd();

		std::string getTrailingParam(int pos, const std::string &msg);
		void logNewMessage();
};

