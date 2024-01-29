#include "Client.hpp"

Client::Client(const std::string &pAddress): address(pAddress) 
{
	Log::out("new connection with " + this->address);
	this->addMsg("Welcome to IRC");
}

Client::~Client()
{
	Log::out(this->address + " disconnected");
}

void Client::addMsg(const std::string &msg)
{
	this->outputQueue.push(msg);
}

std::string Client::getMsg()
{
	std::string retval = "";
	if (this->outputQueue.empty())
		return retval;
	retval = this->outputQueue.front();
	this->outputQueue.pop();
	return retval;
}

void Client::updateMsg(const std::string &message)
{
	this->inputMsg += message;
}

void Client::setNickname(const std::string &pNickname)
{
	nickname = pNickname;
	Log::out("New user nickname " + pNickname);
}

std::string Client::getNextMessage()
{
	int pos;
	std::string msg;
	std::string sep(SEP);

	if ((pos = this->inputMsg.find_first_of(sep), 0) == std::string::npos)
	{
		if (this->inputMsg.length() >= 10 * MAX_MSG_SIZE)
		{
			this->inputMsg = std::string();
			Log::err("Message exceeded maximum allowed size client " + this->address , 0);
		}

		return std::string();
	}
	msg = this->inputMsg.substr(0, pos);
	this->inputMsg.erase(0, pos + sep.length());
	if (msg.length() >= 512)
		return msg.substr(0, 512);
	return msg;
}
