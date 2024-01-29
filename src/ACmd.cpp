#include "ACmd.hpp"

ACmd::ACmd(const int &pFd, const std::string &pMessage) :
	fd(pFd)
{
	std::stringstream streamMsg(pMessage);
	std::string tmp;

	if (pMessage.length() <= 0 || pMessage.length() > 512)
		return;
	if (pMessage[0] == ':')
		std::getline(streamMsg, this->prefix, ' ');
	std::getline(streamMsg, this->command, ' ');
	while (std::getline(streamMsg, tmp, ' '))
	{
		if (tmp[0] == ':')
		{
			this->params.push_back(this->getTrailingParam(streamMsg.tellg(), pMessage));
			break;
		}
		else
			this->params.push_back(trimString(tmp));
	}
	this->logNewMessage();
}

std::string ACmd::getTrailingParam(int pos, const std::string &msg)
{
	std::string retval;

	try
	{
		retval = msg.substr(pos + 1, msg.length());
	}
	catch(std::exception e)
	{
		return std::string();
	}
	return trimString(retval);
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

ACmd::ACmd(const ACmd &other) : fd(other.fd)
{
}

ACmd &ACmd::operator=(const ACmd &other)
{
	(void) other;
	return *this;
}
