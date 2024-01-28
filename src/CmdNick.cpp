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
	std::stringstream ssMsg(this->message);
	std::string str;
	std::getline(ssMsg, str, ' ');
	std::getline(ssMsg, str);
	return str;
}
