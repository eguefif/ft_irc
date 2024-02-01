#include "Channel.hpp"

Channel::Channel(std::vector<std::string> &params): name(params[0])
{
	Log::out("new channel " + this->name + " created");
}

Channel::~Channel()
{
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
