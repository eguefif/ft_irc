#include "Client.hpp"

Client::Client(const std::string &pAddress) :
								address(pAddress) 
{
}

Client::~Client()
{
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
		return (this->currCmd);
	return "";
}
