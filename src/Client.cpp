#include "Client.hpp"

Client::Client(const std::string &pAddress, const int &pFd) :
								address(pAddress), cmdGenerator(new CommandGenerator())
{
	this->cmdGenerator->setFd(pFd);	
}

Client::~Client()
{
	delete this->cmdGenerator;
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

ACmd *Client::updateCmd(const std::string &message)
{
	return this->cmdGenerator->update(message);
}
