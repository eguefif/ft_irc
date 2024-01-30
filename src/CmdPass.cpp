#include "CmdPass.hpp"

CmdPass::CmdPass(const int &pFd, const std::string &pMessage, const std::string &pPass): ACmd(pFd, pMessage), password(pPass) {}

void CmdPass::execute(std::map<int, Client *> &clientList)
{
	std::string errorMsg = this->checkError(clientList);

	if (errorMsg.length())
		clientList.find(this->fd)->second->addMsg(errorMsg);
	if (this->params[0] == this->password)
	{
		Log::out("User registered " + clientList.find(this->fd)->second->getAddress());
		clientList.find(this->fd)->second->authenticate();
	}
}

std::string CmdPass::checkError(std::map<int, Client *> &clientList)
{
	(void)clientList;
	if (!this->params.size())
		return this->createErrorMsg(
			ERR_NEEDMOREPARAMS,
			this->getClientNick(clientList),
			ERR_NEEDMOREPARAMS_STR);
	return std::string();
}
const std::string &ACmd::getClientNick(std::map<int, Client *> &clientList) const
{
	return clientList.find(this->fd)->second->getNickname();
}

