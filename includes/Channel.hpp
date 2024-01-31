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

		void addUser(Client *newUser);
		void addOperator(Client *newOperator);

	private:
		Channel();
		Channel	&operator=(const Channel &other);
		Channel(const Channel &other);

		const std::string &name;
//		std::string &topic;

		std::vector<Client *> users;
		std::vector<Client *> operators;
};

