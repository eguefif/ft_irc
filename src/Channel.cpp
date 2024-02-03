#include "Channel.hpp"

Channel::Channel(std::vector<std::string> &params): name(params[0]), inviteOnly(false), channelTopicOp(true), channelMaxSize(0), passwordActivated(false), channelPass("")
{
	Log::out("new channel " + this->name + " created");
}

Channel::~Channel()
{
}

bool Channel::isInviteOnly() const
{
	return this->inviteOnly;
}

bool Channel::isTopicOp() const
{
	return this->channelTopicOp;
}

bool Channel::isInvited(Client * client)
{
	for (std::vector<Client *>::iterator it = this->invited.begin();
		it != this->invited.end(); ++it)
	{
		if (*it == client)
			return true;
	}
	return false;
}

int Channel::getChannelMaxSize() const
{
	return this->channelMaxSize;
}

std::string Channel::getChannelPassword() const
{
	return this->channelPass;
}

int Channel::getUsersSize() const
{
	return this->users.size();
}

void Channel::addUser(Client *newUser)
{
	this->users.push_back(newUser);
	this->greet(newUser);
	this->broadcast(this->newJoinMsg(newUser), newUser);
	Log::out("user " + newUser->getNickname() + " joined channel " + this->name);
}

void Channel::addOperator(Client *newOperator)
{
	this->operators.push_back(newOperator);
}

void Channel::greet(Client *newUser)
{
	newUser->addMsg(this->newJoinMsg(newUser));
	newUser->addMsg(this->getUserNames(newUser));
	newUser->addMsg(this->endOfNames(newUser));
}

void Channel::broadcast(std::string msg, Client *sender)
{
	for (std::vector<Client *>::iterator it = this->users.begin();
			it != this->users.end();
			++it)
	{
		if (*it != sender)
			(*it)->addMsg(msg);
	}
}

void Channel::broadcast(std::string msg)
{
	for (std::vector<Client *>::iterator it = this->users.begin();
			it != this->users.end();
			++it)
		(*it)->addMsg(msg);
}


std::string	Channel::newJoinMsg(Client *newUser)
{
	return (":" + newUser->getNickname() + " JOIN :" + this->name);
}

std::string Channel::getUserNames(Client *newUser)
{
	std::string retval = SERVER_PREFIX + " " + RPL_NAMREPLY + " "
		+ newUser->getNickname() + " = " + this->name + " :";
	for (std::vector<Client *>::iterator it = this->users.begin();
			it != this->users.end();
			++it)
	{
		if (it != this->users.begin())
			retval += " ";
		if (this->isOperator(*it))
				retval += "@";
		retval += (*it)->getNickname();
	}
	return retval;
}

std::string Channel::endOfNames(Client *user)
{
	return (SERVER_PREFIX + " " + RPL_ENDOFNAMES + " "
			+ user->getNickname() + " " + this->name + " :" +  RPL_ENDOFNAMES_STR);
}

bool Channel::isOperator(Client *user)
{
	for (std::vector<Client *>::iterator it = this->operators.begin();
			it != this->operators.end();
			++it)
	{
		if (*it == user)
			return true;
	}
	return false;
}

void Channel::removeClient(Client *user)
{
	for (std::vector<Client *>::iterator it = this->operators.begin();
			it != this->operators.end();
			++it)
	{
		if (*it == user)
		{
			this->operators.erase(it);
			break;
		}
	}
	for (std::vector<Client *>::iterator it = this->users.begin();
			it != this->users.end();
			++it)
	{
		if (*it == user)
		{
			this->users.erase(it);
			break;
		}
	}
}

void Channel::setInviteOnly(bool toSet)
{
	this->inviteOnly = toSet;
}

void Channel::setTopicOp(bool toSet)
{
	this->channelTopicOp = toSet;
}

void Channel::setPassword(bool toSet, std::string pPassword)
{
	if (this->passwordActivated && this->channelPass == pPassword)
	{
		this->passwordActivated = toSet;
		if (pPassword.length())
			this->channelPass = pPassword;
	}
	if (!this->passwordActivated && toSet)
	{
		if (pPassword.length())
		{
			this->passwordActivated = toSet;
			this->channelPass = pPassword;
		}
	}
}

void Channel::setOperators(bool toSet, std::string oOperators)
{
	(void) toSet;
	(void) oOperators;
}

void Channel::setLimit(bool toSet, std::string limit)
{
	if (toSet == false)
		this->channelMaxSize = 0;
	else
	{
		if (limit.length())
		{
			try
			{
				this->channelMaxSize = std::stoi(limit);
			}
			catch(std::exception &e)
			{
				Log::err("MODE +l limit: " + limit, &e);
			}
		}
	}
}

void Channel::broadCastFlags()
{
	std::string msg = "MODE " + this->name + " :";
	std::string endMsg = "[";

	if (this->inviteOnly)
		msg += "i";
	if (this->channelTopicOp)
		msg += "t";
	if (this->channelMaxSize)
	{
		msg += "l";
		endMsg += std::to_string(this->channelMaxSize);
	}
	if (this->passwordActivated)
	{
		msg += "k";
		endMsg += this->channelPass;
	}
	msg += + " " + endMsg + "]";
	this->broadcast(msg);
}
