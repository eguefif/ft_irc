#pragma once

#include "Client.hpp"
#include "ACmd.hpp"

class Client;

class CmdKick: public ACmd
{
	public:
		~CmdKick() {};
		CmdKick(const int &pFd, const std::string &pMessage);

		void execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);
	
	private:
		CmdKick();
		CmdKick(const CmdKick &other);
		CmdKick &operator=(const CmdKick &other);

		std::string checkError(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);
		std::string createKickMsg(std::map<int, Client *> &clientList);
		std::string getReason();
		bool checkUserInChan(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList);
		bool checkUserInChan(std::string user, std::string channelName, std::map<std::string, Channel *> &channelList);
		bool checkUserIsOp(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList);
};
