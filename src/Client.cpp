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

std::string Client::updateCmd(const std::string &message)
{
	this->currCmd += message;
	if (this->currCmd[this->currCmd.length() - 1] == '\n' || this->currCmd.length() >= 512)
	{
		std::string retVal = this->currCmd;
		this->currCmd = "";
		return (retVal);
	}
	return "";
}

void Client::setNickname(const std::string &pNickname)
{
	nickname = pNickname;
	Log::out("New user nickname " + pNickname);
}
