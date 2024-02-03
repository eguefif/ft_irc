#pragma once

#include "ACmd.hpp"
#include <map>
#include <string>

class CmdInvite: public ACmd
{
	public:
		CmdInvite(const int &pFd, const std::string &pMessage);
		~CmdInvite();

		void execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);

	private:
		CmdInvite(const CmdInvite &);
		CmdInvite &operator=(const CmdInvite &);

		std::string checkError(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList);

		bool checkIfNickExist(std::string nickname, std::map<int, Client *> &clientList);
		bool checkUserInChan(Client *user, std::string channelName,
						std::map<std::string, Channel *> &channelList);
		bool checkUserIsOp(Client *user, std::string channelName,
						std::map<std::string, Channel *> &channelList);
		bool isAlreadyInChan(std::string user, std::string channelName,
						std::map<std::string, Channel *> &channelList);
		Client * getClientFromName(std::string name, std::map<int, Client *> &clientList);
};
