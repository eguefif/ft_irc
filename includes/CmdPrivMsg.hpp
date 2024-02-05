#pragma once

#include "defines.hpp"
#include "Client.hpp"
#include "ACmd.hpp"

class Client;

class CmdPrivMsg : public ACmd
{
	public:
		~CmdPrivMsg() {};
		CmdPrivMsg(const int &pFd, const std::string pMessage);

		void execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);

	private:
		CmdPrivMsg();
		CmdPrivMsg(const CmdPrivMsg &other);
		CmdPrivMsg &operator=(const CmdPrivMsg &rhs);

		std::string checkError(std::map<int, Client*> &clientList,
				std::map<std::string, Channel *> &channelList);
		std::string checkErrorSplit(std::map<int, Client*> &clienList,
				std::map<std::string, Channel *> &channelList);
		std::string createMsg(std::map<int, Client*> &clientList);

		Client *getReceiver(std::map<int, Client*> clientList, std::string receiver);
		bool hasDuplicateReceivers();
		std::vector<std::string> receiverList;
};
