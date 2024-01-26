#include "CmdNick.hpp"

CmdNick::CmdNick(const int &pFd,
				const std::string pMessage) :
	ACmd(pFd, pMessage) {}

void CmdNick::execute(std::map<int, Client *> &clientList)
{
	(void) clientList;
	std::cout << this->message;
}
