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
};
