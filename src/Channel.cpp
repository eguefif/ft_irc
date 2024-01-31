#include "Channel.hpp"

Channel::Channel(std::vector<std::string> &params): name(params[0])
{
}

Channel::~Channel()
{
}

void Channel::addUser(Client *newUser)
{
	this->users.push_back(newUser);
}

void Channel::addOperator(Client *newOperator)
{
	this->operators.push_back(newOperator);

}
