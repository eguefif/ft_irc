#include "Client.hpp"

Client::Client(const std::string &pAddress):
			nickname("*"), address(pAddress),
			authenticated(false), nickRegistered(false), userRegistered(false),
			isConnected(true)
{
	Log::out("new connection with " + this->address);
}

Client::~Client()
{
	Log::out(this->address + " disconnected");
}

const std::string &Client::getAddress() const
{
	return this->address;
}

void Client::addMsg(const std::string &msg)
{
	this->outputQueue.push(msg);
}

void Client::addMsgNonRegistered(const std::string &msg)
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

std::string Client::getNextMessage()
{
	size_t pos;
	std::string msg;
	std::string sep(EOM);

	pos = this->inputMsg.find_first_of(sep);
	if (pos == std::string::npos)
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

void Client::authenticate()
{
	this->authenticated = true;
}

bool Client::isAuthenticated() const
{
	return this->authenticated;
}

void Client::setUser(const std::string &pUser, const std::string &pRealname)
{
	this->user = pUser;
	this->realName = pRealname;
	this->userRegistered = true;
	Log::out("user set to " + pUser + " (" + pRealname + ")" + " " + this->address);
}
void Client::setNickname(const std::string &pNickname)
{
	nickname = pNickname;
	this->nickRegistered = true;
	Log::out("user nickname set to " + pNickname
			+ " " + this->address);
}

const std::string &Client::getNickname() const
{
	return this->nickname;
}


bool Client::isRegistered() const
{
	return this->authenticated && this->nickRegistered && this->userRegistered;
}

void Client::disconnect()
{
	this->isConnected = false;
	this->userRegistered = false;
	this->nickRegistered = false;
	this->authenticated = false;
}

bool  Client::isDisconnected() const
{
	return !(this->isConnected);
}
