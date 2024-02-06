#include "Channel.hpp"
#include <vector>

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

std::string Channel::getChannelName() const
{
	return this->name;
}

std::string Channel::getTopic() const
{
	return this->topic;
}

void Channel::setTopic(const std::string &pTopic)
{
	this->topic = pTopic;
}

int Channel::getUsersSize() const
{
	return this->users.size();
}

void Channel::addUser(Client *newUser)
{
	if (this->isUserInChan(newUser))
		return;
	this->users.push_back(newUser);
	this->greet(newUser);
	this->broadcast(this->newJoinMsg(newUser), newUser);
	Log::out("user " + newUser->getNickname() + " joined channel " + this->name);
}

void Channel::addUserInvited(Client *newUserInvited)
{
	this->invited.push_back(newUserInvited);
}

void Channel::addOperator(Client *newOperator)
{
	this->operators.push_back(newOperator);
}

void Channel::greet(Client *newUser)
{
	newUser->addMsg(this->newJoinMsg(newUser));
	if (this->topic.length())
		newUser->addMsg(this->getTopicMsg(newUser->getNickname()));
	newUser->addMsg(this->getUserNames(newUser));
	newUser->addMsg(this->endOfNames(newUser));
}

std::string Channel::getTopicMsg(std::string nickname)
{
	std::string retval;
	retval += SERVER_PREFIX + " " + RPL_TOPIC;
	retval += " " + nickname;
	retval += " " + this->name;
	retval += " :" + this->topic;
	return retval;
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

void Channel::removeClient(std::string nickname)
{
	for (std::vector<Client *>::iterator it = this->operators.begin();
			it != this->operators.end();
			++it)
	{
		if ((*it)->getNickname() == nickname)
		{
			this->operators.erase(it);
			break;
		}
	}
	for (std::vector<Client *>::iterator it = this->users.begin();
			it != this->users.end();
			++it)
	{
		if ((*it)->getNickname() == nickname)
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
	if (!toSet && this->passwordActivated && this->channelPass == pPassword)
	{
		this->passwordActivated = toSet;
		this->channelPass = "";
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

bool Channel::setOperators(bool toSet, std::string oOperator)
{
	if (toSet && this->isUserInChan(oOperator))
	{
		Client *newop = 0;
		for (std::vector<Client *>::iterator it = this->users.begin();
				it != this->users.end();
				++it)
		{
			if ((*it)->getNickname() == oOperator)
			{
				newop = *it;
				break;
			}
		}
		if (newop)
		{
			this->operators.push_back(newop);
			Log::out("channel " + this->name + " operator: " + oOperator);
		}
		else
			return false;
	}
	else
	{
		bool flag = false;

		for (std::vector<Client *>::iterator it = this->operators.begin();
				it != this->operators.end();
				++it)
		{
			if ((*it)->getNickname() == oOperator)
			{
				this->operators.erase(it);
				Log::out("channel " + this->name + " operator removed: " + oOperator);
				flag = true;
				break;
			}
		}
		if (!flag)
			return false;
	}
	return true;
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
				Log::out("channel " + this->name + " limit set to " + limit);
			}
			catch(std::exception &e)
			{
				Log::err("MODE +l limit: " + limit, &e);
			}
		}
	}
}

bool Channel::isUserInChan(Client *user)
{
	for (std::vector<Client *>::iterator it = this->users.begin(); it != this->users.end(); ++it)
	{
		if (*it == user)
			return true;
	}
	return false;
}

bool Channel::isUserOp(Client *user)
{
	for (std::vector<Client *>::iterator it = this->operators.begin(); it != this->operators.end(); ++it)
	{
		if (*it == user)
			return true;
	}
	return false;
}

bool Channel::isUserInChan(std::string user)
{
	for (std::vector<Client *>::iterator it = this->users.begin(); it != this->users.end(); ++it)
	{
		if ((*it)->getNickname() == user)
			return true;
	}
	return false;
}

void Channel::removeInvited(Client *user)
{
	for (std::vector<Client *>::iterator it = this->invited.begin();
			it != this->invited.end();
			++it)
	{
		if (*it == user)
		{
			this->invited.erase(it);
			break;
		}
	}
}

std::string Channel::getModeString()
{
	std::string retval;

	retval += "+";
	if (this->inviteOnly)
		retval += "i";
	if (this->channelTopicOp)
		retval += "t";
	if (this->channelMaxSize)
		retval += "l";
	if (this->passwordActivated)
		retval += "k";
	return retval;
}
