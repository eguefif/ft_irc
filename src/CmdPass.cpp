#include "CmdPass.hpp"

CmdPass::CmdPass(const int &pFd, const std::string &pMessage, const std::string &pPass): ACmd(pFd, pMessage), password(pPass) {}

void CmdPass::execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList)
{
	std::string errorMsg = this->checkError(clientList, channelList);

	if (errorMsg.length())
		clientList.find(this->fd)->second->addMsg(errorMsg);
	else if (this->params[0] == this->password)
	{
		Log::out("user authenticated " + this->getClientAddr(clientList));
		clientList.find(this->fd)->second->authenticate();
	}
}

std::string CmdPass::checkError(std::map<int, Client *> &clientList,
			std::map<std::string, Channel *> &channelList)
{
	(void) channelList;
	if (!this->params.size())
	{
		return this->createErrorMsg(
			ERR_NEEDMOREPARAMS,
			this->getClientNick(clientList) + " " + "PASS",
			ERR_NEEDMOREPARAMS_STR);
	}
	else if (this->params[0] != this->password || this->params.size() != 1)
	{
		return this->createErrorMsg(
			ERR_PASSWDMISMATCH,
			this->getClientNick(clientList),
			ERR_PASSWDMISMATCH_STR);
	}
	else if (clientList.find(this->fd)->second->isAuthenticated())
	{
		return this->createErrorMsg(
			ERR_ALREADYREGISTRED,
			this->getClientNick(clientList),
			ERR_ALREADYREGISTRED_STR);
	}
	return std::string();
}
