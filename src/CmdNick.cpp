#include "CmdNick.hpp"

CmdNick::CmdNick(const int &pFd, const std::string pMessage): ACmd(pFd, pMessage)
{
}

void CmdNick::execute(std::map<int, Client *> &clientList)
{
	std::string errorMsg = this->checkError(clientList);
	if (errorMsg.length())	
		clientList.find(this->fd)->second->addMsg(errorMsg);
	else
	{
		if (!clientList.find(this->fd)->second->isRegistered())
		{
				clientList.find(this->fd)->second->setNickname(this->getNewNickname());
				if (clientList.find(this->fd)->second->isRegistered())
				{
					clientList.find(this->fd)->second->addMsg("Welcome to IRC!");
					Log::out("client registered: "
							+ this->getClientNick(clientList)
							+ " "
							+ this->getClientAddr(clientList));
				}
		}
		else
				clientList.find(this->fd)->second->setNickname(this->getNewNickname());
	}
}

std::string CmdNick::checkError(std::map<int, Client *> &clientList)
{
	if (!this->isClientAuthenticated(clientList))
		return (this->createErrorMsg(
				ERR_NOTREGISTERED,
				this->getClientNick(clientList),
				ERR_NOTREGISTERED_STR));
	if (!this->params.size())
		return (this->createErrorMsg(
					ERR_NONICKNAMEGIVEN,
					"",
					ERR_NONICKNAMEGIVEN_STR));
	if (this->params.size() > 1 || this->params[0].length() > 9)
		return (this->createErrorMsg(
				ERR_ERRONEUSNICKNAME,
				this->getClientNick(clientList),
				ERR_ERRONEUSNICKNAME_STR));
	if (!checkNickUnicity(clientList))
		return (this->createErrorMsg(
				ERR_NICKNAMEINUSE,
				this->getClientNick(clientList),
				ERR_NICKNAMEINUSE_STR));
	if (!isWord(this->getNewNickname()))
		return this->createErrorMsg(
				ERR_INVALIDCHAR,
				this->getClientNick(clientList),
				ERR_INVALIDCHAR_STR);

	return std::string();
}

bool CmdNick::checkNickUnicity(std::map<int, Client *> &clientList)
{
	std::string tmpNick = this->getNewNickname();
	for (std::map<int, Client*>::iterator it = clientList.begin();
			it != clientList.end();
			++it)
		if (it->second->getNickname() == tmpNick)
			return false;
	return true;
}

const std::string CmdNick::getNewNickname()
{
	if (this->params.size())
		return this->params[0];
	else
		return std::string("*");
}
