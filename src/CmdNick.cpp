#include "CmdNick.hpp"

CmdNick::CmdNick(const int &pFd, const std::string pMessage): ACmd(pFd, pMessage)
{
}

void CmdNick::execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList)
{
	std::string errorMsg = this->checkError(clientList, channelList);

	if (errorMsg.length())	
		clientList.find(this->fd)->second->addMsgNonRegistered(errorMsg);
	else
	{
		if (!clientList.find(this->fd)->second->isRegistered())
		{
			clientList.find(this->fd)->second->setNickname(this->getNewNickname());
			if (clientList.find(this->fd)->second->isRegistered())
				{
					std::string welcomeMsg = this->createReplyMsg(RPL_WELCOME, this->getNewNickname(),
						   	RPL_WELCOME_STR);
					clientList.find(this->fd)->second->addMsg(welcomeMsg);
					Log::out("client registered: "
							+ this->getClientNick(clientList)
							+ " "
							+ this->getClientAddr(clientList));
				}
		}
		else
		{
				clientList.find(this->fd)->second->setNickname(this->getNewNickname());
				std::string confirmNickChg = this->createReplyMsg(RPL_WELCOME, this->getNewNickname(),
						RPL_CONFIRM_NICK_CHANGE);
				clientList.find(this->fd)->second->addMsg(confirmNickChg);
		}
	}
}

std::string CmdNick::checkError(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList)
{
	(void) channelList;

	if (!this->isClientAuthenticated(clientList))
		return (this->createErrorMsg(
				ERR_NOTREGISTERED,
				this->getClientNick(clientList),
				ERR_NOTREGISTERED_STR));
	if (!this->params.size())
		return (this->createErrorMsg(
				ERR_NONICKNAMEGIVEN,
				this->getClientNick(clientList),
				ERR_NONICKNAMEGIVEN_STR));
	if (this->params.size() > 1 || this->params[0].length() > 9)
		return (this->createErrorMsg(
				ERR_ERRONEUSNICKNAME,
				this->getClientNick(clientList) + " " + getStringParams(),
				ERR_ERRONEUSNICKNAME_STR));
	if (!checkNickUnicity(clientList))
		return (this->createErrorMsg(
				ERR_NICKNAMEINUSE,
				this->getClientNick(clientList)  + " " + this->getNewNickname(),
				ERR_NICKNAMEINUSE_STR));
	if (!isWord(this->getNewNickname()) ||( this->getNewNickname()[0] > '0' && this->getNewNickname()[0] < '9'))
		return this->createErrorMsg(
				ERR_INVALIDCHAR,
				this->getClientNick(clientList) + " " + getStringParams(),
				ERR_INVALIDCHAR_STR);
	if (this->getNewNickname()[0] == '#' || this->getNewNickname()[0] == ':')
		return this->createErrorMsg(
				ERR_INVALIDCHAR,
				this->getClientNick(clientList) + " " + getStringParams(),
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
