#include "CmdPrivMsg.hpp"

CmdPrivMsg::CmdPrivMsg(const int &pFd, const std::string pMessage): ACmd(pFd, pMessage) 
{
	if (this->params.size() >= 1)
	{
		std::stringstream paramCmd(this->params[0]);
		std::string recipient;

		while (std::getline(paramCmd, recipient, ','))
		{
			this->receiverList.push_back(recipient);
		}
	}
}

void CmdPrivMsg::execute(std::map<int, Client *> &clientList,
		std::map<std::string, Channel *> &channelList)
{
	if (!this->isClientRegistered(clientList))
	{
		clientList.find(this->fd)->second->addMsgNonRegistered(this->createErrorMsg(
					ERR_NOTREGISTERED,
					this->getClientNick(clientList),
					ERR_NOTREGISTERED_STR));
	}
	std::string errorMsg = this->checkError(clientList, channelList);
	if (errorMsg.length())
	{
		clientList.find(this->fd)->second->addMsg(errorMsg);
		return;
	}
	for (std::vector<std::string>::iterator it = this->receiverList.begin();
			it != this->receiverList.end();
			++it)
	{
		this->params[0] = *it; 
		errorMsg = std::string();
		errorMsg = this->checkErrorSplit(clientList, channelList);
		if (errorMsg.length())
		{
			clientList.find(this->fd)->second->addMsg(errorMsg);
			continue;
		}
		std::string message = this->createMsg(clientList);
		if (this->params[0][0] == '#')
		{
			channelList.find(this->params[0])->second->broadcast(message,
					clientList.find(this->fd)->second);
		}
		else
		{
			this->getReceiver(clientList, this->params[0])->addMsg(message);
		}
		Log::out("PRIVMSG sent from " + this->getClientNick(clientList)
				+ " to " + this->params[0]);
	}
}

std::string CmdPrivMsg::checkError(std::map<int, Client *> &clientList,
		std::map<std::string, Channel *> &channelList)
{
	(void) channelList;
   	if (this->params.size() < 1)
	{
		return (this->createErrorMsg(
			ERR_NORECIPIENT,
			this->getClientNick(clientList), 
			ERR_NORECIPIENT_STR));
	}
   	if (this->params.size() < 2)
	{
		return (this->createErrorMsg(
			ERR_NOTEXTTOSEND,
			this->getClientNick(clientList), 
			ERR_NOTEXTTOSEND_STR));
	}
	if (this->hasDuplicateReceivers())
	{
		return (this->createErrorMsg(
			ERR_TOOMANYTARGETS,
			this->getClientNick(clientList), 
			ERR_TOOMANYTARGETS_STR));
	}
	return  std::string();
}

std::string CmdPrivMsg::checkErrorSplit(std::map<int, Client *> &clientList,
		std::map<std::string, Channel *> &channelList)
{
	std::map<std::string, Channel *>::iterator currentChanIt = channelList.find(this->params[0]);
	Client *receiver = this->getReceiver(clientList, this->params[0]);
	if (this->params[0][0] == '#')
	{
		std::cout << this->params[0] << std::endl;
		if (currentChanIt == channelList.end())
		{
			return (this->createErrorMsg(
				ERR_NOSUCHCHANNEL,
				this->getClientNick(clientList) + " " + this->params[0],
				ERR_NOSUCHCHANNEL_STR));
		}
		if (!currentChanIt->second->isUserInChan(clientList.find(this->fd)->second))
		{
			return (this->createErrorMsg(
				ERR_CANNOTSENDTOCHAN,
				this->getClientNick(clientList) + " " + this->params[0],
				ERR_CANNOTSENDTOCHAN_STR));
		}
	}
	else if (!receiver || !receiver->isRegistered())
	{
		return (this->createErrorMsg(
			ERR_NOSUCHNICK,
			this->getClientNick(clientList) + " " + this->params[0],
			ERR_NOSUCHNICK_STR));
	}
	return  std::string();
}

std::string CmdPrivMsg::createMsg(std::map<int, Client*> &clientList)
{
	std::string message = this->getClientNick(clientList) + " PRIVMSG "
		+ this->params[0] + " :";
	for (std::vector<std::string>::iterator it = std::next(params.begin());
			it != params.end();
			++it)
	{
		if (it != std::next(params.begin()))
			message += " ";
		message += *it;
	}
	return message;
}

bool CmdPrivMsg::hasDuplicateReceivers()
{
	for (std::vector<std::string>::iterator it = this->receiverList.begin();
			it != this->receiverList.end();
			++it)
	{
		for (std::vector<std::string>::iterator nextIt = std::next(it);
				nextIt != this->receiverList.end();
				++nextIt)
		{
			if (*nextIt == *it)
				return true;
		}
	}
	return false;
}

Client *CmdPrivMsg::getReceiver(std::map<int, Client*> clientList, std::string receiver)
{
	for (std::map<int, Client*>::iterator it = clientList.begin();
			it != clientList.end();
			++it)
	{
		if (it->second->getNickname() == receiver)
		{
			return(it->second);
		}
	}
	return 0;
}
