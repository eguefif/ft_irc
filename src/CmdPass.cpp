#include "CmdPass.hpp"

CmdPass::CmdPass(const int &pFd, const std::string &pMessage, const std::string &pPass): ACmd(pFd, pMessage), password(pPass) {}

void CmdPass::execute(std::map<int, Client *> &clientList)
{
	std::string errorMsg = this->checkError(clientList);
	if (this->params[0] == this->password)
	{
		Log::out("User registered " + clientList.find(this->fd)->second->getAddress());
		clientList.find(this->fd)->second->setRegistered();
	}
}

std::string CmdPass::checkError(std::map<int, Client *> &clientList)
{
	(void)clientList;
	return std::string();
}
