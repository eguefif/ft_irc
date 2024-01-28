#pragma once

#include <sstream>
#include <map>

#include "Client.hpp"

class Client;

class ACmd
{
	public:
		ACmd(const int &pFd, const std::string pMessage);
		ACmd(const ACmd &other); 
		ACmd &operator=(const ACmd &other);
		virtual ~ACmd() {};

		virtual void execute(std::map<int, Client *> &clientList) = 0;
	
	protected:
		const int fd;
		const std::string message;

	private:
		ACmd();
};
