#include "../includes/CmdInvite.hpp"


CmdInvite::CmdInvite(const int &pFd, const std::string &pMessage): ACmd(pFd, pMessage)
{
}

CmdInvite::~CmdInvite()
{
}

void CmdInvite::execute(std::map<int, Client *> &clientList, std::map<std::string, Channel *> &channelList)
{

}

std::string CmdInvite::checkError(std::map<int, Client *> &clientList, std::map<std::string, Channel *> &channelList)
{
	if (this->params.size() > 3)
		return ;
	return std::string();
}
