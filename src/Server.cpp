#include "Server.hpp"

Server::Server(const std::string &pPort, const std::string &pPass)
{
	this->pass = pPass;
	this->numSockets = 0;
	try
	{
		this->port = std::stoi(pPort);
	}
	catch(std::exception & e)
	{
		Log::err("port param error", &e);
	}
	if (port < 0 || port > 49151)
		Log::err("port is out of range [0;49151]", 0);
	serverSocket = this->initConnection(this->addressServer);
	this->initPoll();
}

Server::~Server()
{
	for (nfds_t i = 0; i < this->numSockets; ++i)
		close(this->pfds[i].fd);
	delete[] this->pfds;
}

int	Server::initConnection(struct sockaddr_in &address)
{
	int newSocket;
	int opt = 1;

	if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		Log::err("socket failed", 0);
		exit(1);
	}
	if (setsockopt(newSocket, SOL_SOCKET,
				SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		Log::err("setsockopt failed", 0);
		exit(1);
	}
	address.sin_family = AF_INET;
	if (!inet_aton("0.0.0.0", &(address.sin_addr)))
	{
		Log::err("invalid IP address", 0);
		exit(1);
	}
	address.sin_port = htons(this->port);
	if (bind(newSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
	{
		Log::err("bind failed", 0);
		exit(1);
	}
	if (listen(newSocket, 10) < 0)
	{
		Log::err("listen failed", 0);
		exit(1);
	}
	setNonBlockingSocket(newSocket);
	this->numSockets++;
	return newSocket;
}

void setNonBlockingSocket(const int &fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		Log::err("fcntl error: " + std::to_string(errno), 0);
}

void Server::run()
{
	while (true)
	{
		if (poll(this->pfds, this->numSockets, -1) < 0)
		{
			Log::err("poll failure", 0);
			exit(1);
		}
		if (this->pfds[0].revents & POLLIN)
		{
			this->newConnection();
		}
		for(nfds_t i = 1; i < this->numSockets; ++i)
		{
			if (this->pfds[i].revents & POLLIN)
			{
				std::string cmdTmp = "";
				char buffer[MAX_MSG_SIZE];
				bzero(buffer, MAX_MSG_SIZE);
				int retVal;
				while ((retVal = recv(this->pfds[i].fd, buffer, MAX_MSG_SIZE, 0)) > 0)
					cmdTmp = this->clientList.find(this->pfds[i].fd)->second->updateCmd(buffer);
				if (retVal == 0)
				{
//					this->closedPfdsIndex.push_back(i);
					this->removeClient(this->pfds[i].fd, i);
				}
				if (cmdTmp.length() != 0)
					this->cmdList.push_back(cmdFactory(cmdTmp, this->pfds[i].fd));
			}
		}
/*		for (int i = 0; i < (int)fdToRemove.size(); ++i)
		{
			this->pfds  + fdToRemove.at(i));
		}
		*/
		for (std::vector<ACmd *>::iterator it = this->cmdList.begin();
				it != this->cmdList.end(); ++it)
		{
			if (*it)
				(*it)->execute(this->clientList);
			delete (*it);
		}
		this->cmdList.clear();
		for(nfds_t i = 0; i < this->numSockets; ++i)
		{
			if (this->pfds[i].revents & POLLOUT)
			{
				std::string message;
				//if ()
				while((message = this->clientList.find(pfds[i].fd)->second->getMsg()) != "")
				{
					write(pfds[i].fd, message.c_str(), message.length());
					write(pfds[i].fd, "\n", 1);
				}
			}
		}
	}
	close(this->serverSocket);
}

void Server::initPoll()
{
	this->pfds = new pollfd[SERV_MAX_CLIENTS];
	this->pfds[0].fd = this->serverSocket;
	this->pfds[0].events = POLLIN;
}

void Server::newConnection()
{
	int	fd = 0;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(sockaddr);
	fd = accept(this->serverSocket, (struct sockaddr *)&address, &addrlen);
	setNonBlockingSocket(fd);
	std::string clientAddress(inet_ntoa(address.sin_addr));
	//Log::out("new connection with " + clientAddress + ":" + std::to_string(ntohs(this->addressServer.sin_port)));
	this->pfds[this->numSockets].fd = fd;
	this->pfds[this->numSockets].events = POLLIN | POLLOUT;
	this->clientList.insert(std::pair<int, Client*>(fd, new Client(clientAddress)));
	//this->clientList.find(fd)->second->addMsg("Welcome to IRC");
	this->numSockets++;
}

void Server::removeClient(const int &fd, int index)
{
	delete this->clientList.find(fd)->second;
	this->clientList.erase(fd);
	close(this->pfds[index].fd);
}
