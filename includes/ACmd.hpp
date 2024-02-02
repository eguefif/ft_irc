#pragma once

#include <sstream>
#include <map>
#include <vector>
#include <exception>

#include "defines.hpp"
#include "stringUtils.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Client;

class ACmd
{
	public:
		ACmd(const int &pFd, const std::string &pMessage);
		ACmd(const ACmd &other); 
		ACmd &operator=(const ACmd &other);
		virtual ~ACmd() {};

		virtual void execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList) = 0;
 	
	protected:
		const int fd;
		std::string prefix;
		std::string command;
		std::vector<std::string> params;

		const std::string &getClientNick(std::map<int, Client *> &clientList) const;
		const std::string &getClientAddr(std::map<int, Client *> &clientList) const;
		bool isClientAuthenticated(std::map<int, Client *> &clientList) const;
		bool isClientRegistered(std::map<int, Client *> &clientList) const;
		std::string createErrorMsg(std::string num, std::string nickname, std::string error);
		std::string createReplyMsg(const std::string &num, const std::string &nickname, const std::string &msg);
		virtual std::string checkError(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList) = 0;
		std::string getStringParams();

	private:
		ACmd();

		std::string getTrailingParam(int pos, const std::string &msg);
		void logNewMessage();
};
