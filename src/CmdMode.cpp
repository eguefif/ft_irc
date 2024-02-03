#include "CmdMode.hpp"

CmdMode::CmdMode(const int &pFd, const std::string &pMessage): ACmd(pFd, pMessage)
{
	if (this->params.size() < 2)
		return;
	if (this->params[0].length() < 2)
		return;
	else
	{
		if (this->params[0][0] != '#')
			return ;
		else
			this->channel = std::string(this->params[0]);
	}
	for (std::vector<std::string>::iterator it = this->params.begin() + 1;
			it != this->params.end();
			++it)
	{
		if (it->length() >= 1 && ((*it)[0] != '-' || (*it)[0] != '+'))
		{
			this->args.push_back(*it);
			continue;
		}
		if (it->length() == 2)
			flags.push_back(*it);
		else if (it->length() >= 2)
			this->addManyFlags(*it);
	}
}

void CmdMode::addManyFlags(std::string str)
{
	bool toSet;

	for(std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		if (*it == '+' || *it == '-')
		{
			if (*it == '+')
				toSet = true;
			if (*it == '-')
				toSet = false;
		}
		else
		{
			if (MODE_LETTERS.find(*it) != std::string::npos)
			{
				if (toSet)
					flags.push_back("+" + std::to_string(*it));
				else
					flags.push_back("-" + std::to_string(*it));
			}
		}
	}
}

void CmdMode::execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList)
{
	(void) clientList;
	std::string errorMsg = this->checkError(clientList, channelList);

	if (errorMsg.length())
		clientList.find(this->fd)->second->addMsgNonRegistered(errorMsg);
	else 
	{
		for (std::vector<std::string> it = this->flags.begin();
				it != this->flags.end();
				++it)
		{
			this->handleFlags(*it, channelList);
		}

	}
}

std::string CmdMode::checkError(std::map<int, Client *> &clientList,
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
	else if (clientList.find(this->fd)->second->isRegistered())
	{
		return this->createErrorMsg(
			ERR_ALREADYREGISTRED,
			this->getClientNick(clientList),
			ERR_ALREADYREGISTRED_STR);
	}
	return std::string();
}

void CmdMode::handleFlag(std::string str, std::map<std::string, Channel*> channelList)
{
	Channel *currentChannel = channelList.find(channelList)->second;

	currentChannel.setFlags(str);
}
