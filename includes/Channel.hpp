#pragma once

#include <queue>
#include <string>

#include "defines.hpp"
#include "Log.hpp"
#include "Client.hpp"

class Channel
{
	public:
		Channel(std::vector<std::string> &params);
		~Channel();

		bool isInviteOnly() const;
		bool isTopicOp() const;
		bool isInvited(Client *client);
		int getChannelMaxSize() const;
		std::string getChannelPassword() const;
		int getUsersSize() const;

		void setFlags(std::string flag);
		void addUser(Client *newUser);
		void addOperator(Client *newOperator);
		void broadcast(std::string msg, Client *sender);
		void greet(Client *newUser);
		void removeClient(Client *user);

	private:
		Channel();
		Channel	&operator=(const Channel &other);
		Channel(const Channel &other);

		const std::string name;
//		std::string &topic;
		std::vector<Client *> users;
		std::vector<Client *> operators;
		std::vector<Client *> invited;

		bool inviteOnly;
		bool channelTopicOp;
		int channelMaxSize;
		std::string channelPass;

		std::string	newJoinMsg(Client *newUser);
		std::string getUserNames(Client *newUser);
		std::string endOfNames(Client *user);
		bool isOperator(Client *user);
};

