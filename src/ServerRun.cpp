#include "Server.hpp"

void Server::run()
{
	while (true)
	{
		this->runPoll();
		this->handleNewconnection();
		this->handlePauline();
		this->runCommands();
		this->removeClosedConnections();
		this->handlePollout();
	}
	close(this->serverSocket);
}

void Server::runPoll()
{
	if (poll(this->pfds.data(), this->numSockets, -1) < 0)
	{
		Log::err("poll failure", 0);
		exit(1);
	}
}

void Server::handleNewconnection()
{
	if (this->pfds[0].revents & POLLIN)
	{
		this->newConnection();
	}
}

void Server::newConnection()
{
	int	fd = 0;
	struct pollfd newPfds;
	struct sockaddr_in address;
	std::string clientAddress;
	socklen_t addrlen;
	
	addrlen = sizeof(sockaddr);
	fd = accept(this->serverSocket, (struct sockaddr *)&address, &addrlen);
	this->setNonBlockingSocket(fd);
	clientAddress = inet_ntoa(address.sin_addr);
	newPfds.fd = fd;
	newPfds.events = POLLIN | POLLOUT;
	this->pfds.push_back(newPfds);
	this->clientList.insert(std::pair<int, Client*>(fd, new Client(clientAddress)));
	this->numSockets++;
}

void Server::handlePauline()
{
	this->listenPauline();
	this->removeClosedConnections();
}

void Server::listenPauline()
{
	std::string cmdTmp;
	
	for(nfds_t i = 1; i < this->numSockets; ++i)
	{
		if (this->pfds[i].revents & POLLIN)
		{
			cmdTmp = this->readSocket(this->pfds[i].fd);
			if (cmdTmp.length())
				this->clientList.find(this->pfds[i].fd)->second->updateMsg(cmdTmp);
		}
	}
}

std::string Server::readSocket(const int &fd)
{
	char buffer[MAX_MSG_SIZE];
	int retVal;

	bzero(buffer, MAX_MSG_SIZE);
	retVal = recv(fd, buffer, MAX_MSG_SIZE, 0);
	if (retVal > 0)
		return std::string(buffer);
	if (this->isReadError(retVal))
		Log::err("read error for socket " + std::to_string(fd) + " errno: " + std::to_string(errno), 0);
	if (this->isConnectionClosed(retVal))
		this->closedPfdsIndex.push_back(fd);
	return std::string();
}

bool Server::isReadError(const int &retVal) const
{
	return retVal == -1 && errno != EAGAIN && errno != EWOULDBLOCK && errno != ECONNRESET;
}

bool Server::isConnectionClosed(const int &retVal) const
{
	return retVal == 0;
}

void Server::removeClosedConnections()
{
	for (std::map<int, Client *>::iterator it = this->clientList.begin();
			it != this->clientList.end();
			++it)
	{
		if (it->second->isDisconnected())
			this->closedPfdsIndex.push_back(it->first);
	}
	for (int i = 0; i < (int)this->closedPfdsIndex.size(); ++i)
	{
		for (unsigned long j = 0; j < this->pfds.size(); ++j)
		{
			if (this->pfds[j].fd == this->closedPfdsIndex[i])
			{
				this->removeClientFromChannels(this->clientList.find(this->pfds[j].fd)->second);
				this->removeClient(this->pfds[j].fd);
				close(this->pfds[j].fd);
				this->pfds.erase(this->pfds.begin() + j);
				this->numSockets--;
				break ;
			}
		}
	}
	this->closedPfdsIndex.clear();
}

void Server::removeClientFromChannels(Client *user)
{
	for (std::map<std::string, Channel *>::iterator it = channelList.begin();
			it != channelList.end();
			++it)
	{
		it->second->removeClient(user);
	}
}

void Server::removeClient(const int &fd)
{
	std::string message;
	while((message = this->clientList.find(fd)->second->getMsg()) != "")
	{
		write(fd, message.c_str(),
				message.length() > MAX_MSG_SIZE - 2 ? MAX_MSG_SIZE - 2 : message.length());
		write(fd, EOM.c_str(), 2);
	}
	delete this->clientList.find(fd)->second;
	this->clientList.erase(fd);
}

void Server::runCommands()
{
	std::string msg;
	ACmd *cmd;

	for (std::map<int, Client*>::iterator it = this->clientList.begin();
			it != this->clientList.end();
			++it)
	{
		while ((msg = it->second->getNextMessage()).length())
		{
			cmd = cmdFactory(msg, it->first, this->pass);
			if (cmd)
			{
				cmd->execute(this->clientList, this->channelList);
				delete cmd;
			}
		}
	}
}

void Server::handlePollout()
{
	for(nfds_t i = 0; i < this->numSockets; ++i)
	{
		if (this->pfds[i].revents & POLLOUT)
		{
			std::string message;
			while((message = this->clientList.find(pfds[i].fd)->second->getMsg()) != "")
			{
				write(pfds[i].fd, message.c_str(),
							message.length() > MAX_MSG_SIZE - 2 ? MAX_MSG_SIZE - 2 : message.length());
				write(pfds[i].fd, EOM.c_str(), 2);
			}
		}
	}
}
