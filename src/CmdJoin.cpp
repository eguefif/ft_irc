#include "CmdNick.hpp"

CmdNick::CmdNick(const int &pFd, const std::string pMessage): ACmd(pFd, pMessage)
{
}

void CmdNick::execute(std::map<int, Client *> &clientList)
{
	std::string errorMsg = this->checkError(clientList);
	if (errorMsg.length())	
	{
		clientList.find(this->fd)->second->addMsg(errorMsg);
		Log::out("Invalid NICK command: " + clientList.find(this->fd)->second->getAddress());
	}
	else
		clientList.find(this->fd)->second->setNickname(this->getNewNickname());
}

std::string CmdNick::checkError(std::map<int, Client *> &clientList)
{
	if (!this->params.size())
		return (this->createErrorMsg(
					ERR_NONICKNAMEGIVEN,
					"",
					ERR_NONICKNAMEGIVEN_STR));
	if (this->params.size() > 1 || this->params[0].length() > 9)
		return (this->createErrorMsg(
				ERR_ERRONEUSNICKNAME,
				clientList.find(this->fd)->second->getNickname(),
				ERR_ERRONEUSNICKNAME_STR));
	if (!checkNickUnicity(clientList))
		return (this->createErrorMsg(
				ERR_NICKNAMEINUSE,
				clientList.find(this->fd)->second->getNickname(),
				ERR_NICKNAMEINUSE_STR));
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
