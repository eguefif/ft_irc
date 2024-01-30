#include "CmdUser.hpp"

CmdUser::CmdUser(const int &pFd, const std::string &pMessage): ACmd(pFd, pMessage) {}

void CmdUser::execute(std::map<int, Client *> &clientList)
{
	std::string errorMsg = this->checkError(clientList);

	if (errorMsg.length())
		clientList.find(this->fd)->second->addMsg(errorMsg);
	else
	{
		clientList.find(this->fd)->second->setUser(
				this->params[0],
				this->params[3]);
		if (clientList.find(this->fd)->second->isRegistered())
		{
			clientList.find(this->fd)->second->addMsg("Welcome to IRC!");
			Log::out("new client registered: "
					+ this->getClientNick(clientList)
					+ " "
					+ this->getClientAddr(clientList));
		}
	}
}

std::string CmdUser::checkError(std::map<int, Client *> &clientList)
{
	if (!clientList.find(this->fd)->second->isAuthenticated())
	{
		return this->createErrorMsg(
			ERR_NOTREGISTERED,
			this->getClientNick(clientList),
			ERR_NOTREGISTERED_STR);
	}
	else if (clientList.find(this->fd)->second->isRegistered())
	{
		return this->createErrorMsg(
			ERR_ALREADYREGISTRED,
			this->getClientNick(clientList),
			ERR_ALREADYREGISTRED_STR);
	}
	else if (this->params.size() != 4)
	{
		return this->createErrorMsg(
			ERR_NEEDMOREPARAMS,
			this->getClientNick(clientList),
			ERR_NEEDMOREPARAMS_STR);
	}
	else if (!isPrint(this->params[0]) || !isPrint(this->params[3]))
	{
		return this->createErrorMsg(
			ERR_INVALIDCHAR,
			this->getClientNick(clientList),
			ERR_INVALIDCHAR_STR);
	}
	return std::string();
}
