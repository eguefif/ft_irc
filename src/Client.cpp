#include "Client.hpp"

Client::Client(const std::string &pAddress): address(pAddress)
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
