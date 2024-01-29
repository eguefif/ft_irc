#pragma once

#include <sstream>
#include <map>
#include <vector>
#include "stringUtils.hpp"
#include <exception>

#include "defines.hpp"
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
		virtual std::string checkError(std::map<int, Client *> &clientList) = 0;
	
	protected:
		const int fd;
		std::string prefix;
		std::string command;
		std::vector<std::string> params;
		std::string createErrorMsg(int num, std::string nickname, std::string error);

	private:
		ACmd();

		std::string getTrailingParam(int pos, const std::string &msg);
		void logNewMessage();
};
