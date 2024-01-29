#include "CmdNick.hpp"

CmdNick::CmdNick(const int &pFd,
				const std::string pMessage) :
	ACmd(pFd, pMessage) {}

void CmdNick::execute(std::map<int, Client *> &clientList)
{
	clientList.find(this->fd)->second->setNickname(this->getNickname());
}

const std::string CmdNick::getNickname()
{
	if (this->params.size())
	{
		return this->params[0];
	}
	else
		return std::string();
}
