#include "ACmd.hpp"

ACmd::ACmd(const int &pFd, const std::string &pMessage): fd(pFd)
{
	std::stringstream streamMsg(pMessage);
	std::string tmp;

	if (pMessage.length() <= 0 || pMessage.length() > 512)
		return;
	if (pMessage[0] == ':')
		streamMsg >> this->prefix;
	streamMsg >> this->command;
	while (std::getline(streamMsg, tmp, ' '))
	{
		if (!tmp.length())
			continue;
		if (tmp[0] == ':')
		{
			this->params.push_back(this->getTrailingParam(
						(int)streamMsg.tellg() - (int)tmp.length(),
						pMessage));
			break;
		}
		else
			this->params.push_back(trimString(tmp));
	}
}

std::string ACmd::getTrailingParam(int pos, const std::string &msg)
{
	std::string retval;

	try
	{
		retval = msg.substr(pos, msg.length() - pos);
	}
	catch(std::exception e)
	{
		return std::string();
	}
	return retval;
}

void ACmd::logNewMessage()
{
	std::string msg;

	if (this->prefix.length())
		msg = this->prefix + " " + this->command;
	else if (this->command.length())
		msg = this->command;
	if (this->params.size())
		for (std::vector<std::string>::iterator it = this->params.begin(); it != this->params.end(); ++it)
			msg += " " + *it;

	if (msg.length())
		Log::out("New message: " + msg); 
	else
		Log::out("New message: empty"); 
}

std::string ACmd::createErrorMsg(int num, std::string nickname, std::string error)
{
	std::string retval;

	retval += SERVER_PREFIX + " " + std::to_string(num);
	if (nickname.length())
		retval += " " + nickname;
	for (std::vector<std::string>::iterator it = this->params.begin();
			it != this->params.end(); ++it)
		retval += " " + *it;
	retval += " :" + error;
	return retval;
}

const std::string &ACmd::getClientNick(std::map<int, Client *> &clientList) const
{
	return clientList.find(this->fd)->second->getNickname();
}

const std::string &ACmd::getClientAddr(std::map<int, Client *> &clientList) const
{
	return clientList.find(this->fd)->second->getAddress();
}

bool ACmd::isClientAuthenticated(std::map<int, Client *> &clientList) const
{
	return clientList.find(this->fd)->second->isAuthenticated();
}

bool ACmd::isClientRegistered(std::map<int, Client *> &clientList) const
{
	return clientList.find(this->fd)->second->isRegistered();
}

ACmd::ACmd(const ACmd &other) : fd(other.fd)
{
}

ACmd &ACmd::operator=(const ACmd &other)
{
	(void) other;
	return *this;
}
