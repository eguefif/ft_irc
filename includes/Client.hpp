#pragma once

#include <queue>
#include <string>

#include "defines.hpp"
#include "Log.hpp"


class Client
{
	public:
		Client(const std::string &pAddress);
		~Client();

		void addMsg(const std::string &msg);
		void addMsgNonRegistered(const std::string &msg);
		std::string getMsg();

		const std::string &getAddress() const;
		void authenticate();
		bool isRegistered() const;
		bool isAuthenticated() const;
		const std::string &getNickname() const;
		void setNickname(const std::string &nickname);
		void setUser(const std::string &pUser, const std::string &pRealname);
		void disconnect();
		bool isDisconnected() const;

		void updateMsg(const std::string &message);
		std::string getNextMessage();

	private:
		Client();
		Client	&operator=(const Client &other);
		Client(const Client &other);

		std::string inputMsg;
		std::string nickname;
		std::string user;
		std::string realName;
		const std::string address;
		bool authenticated;
		bool nickRegistered;
		bool userRegistered;
		bool isConnected;
		std::queue<std::string> outputQueue;
};
