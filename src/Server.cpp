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
	this->initConnection(this->addressServer);
	this->initPoll();
}

Server::~Server()
{
	for (nfds_t i = 0; i < this->numSockets; ++i)
		close(this->pfds[i].fd);
}

void Server::initConnection(struct sockaddr_in &address)
{
	int opt = 1;

	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		Log::err("socket failed", 0);
		exit(1);
	}
	if (setsockopt(serverSocket, SOL_SOCKET,
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
	if (bind(serverSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
	{
		Log::err("bind failed", 0);
		exit(1);
	}
	if (listen(serverSocket, 10) < 0)
	{
		Log::err("listen failed", 0);
		exit(1);
	}
	setNonBlockingSocket(serverSocket);
	this->numSockets++;
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
		runPoll();
		handleNewconnection();
		handlePauline();
		runCommands();
		handlePollout();
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

void Server::handlePauline()
{
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
				this->closedPfdsIndex.push_back(this->pfds[i].fd);
				this->removeClient(this->pfds[i].fd, i);
			}
			if (cmdTmp.length() != 0)
				this->cmdList.push_back(cmdFactory(cmdTmp, this->pfds[i].fd));
		}
	}
	for (int i = 0; i < (int)this->closedPfdsIndex.size(); ++i)
	{
		int j = 0;
		for (std::vector<pollfd>::iterator it = this->pfds.begin(); it != this->pfds.end(); ++it)
		{
			if ((*it).fd == this->closedPfdsIndex[i])
				break;
			j++;
		}
		if (static_cast<unsigned long>(j) == this->pfds.size())
			continue;
		close(this->pfds[j].fd);
		this->pfds.erase(this->pfds.begin() + j);
		this->numSockets--;
	}
	this->closedPfdsIndex.clear();
}

void Server::runCommands()
{
	for (std::vector<ACmd *>::iterator it = this->cmdList.begin();
			it != this->cmdList.end(); ++it)
	{
		if (*it)
			(*it)->execute(this->clientList);
		delete (*it);
	}
	this->cmdList.clear();
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
				write(pfds[i].fd, message.c_str(), message.length());
				write(pfds[i].fd, "\n", 1);
			}
		}
	}
}

void Server::initPoll()
{
	struct pollfd p;

	p.fd = this->serverSocket;
	p.events = POLLIN;
	this->pfds.push_back(p);
	//this->pfds = new pollfd[SERV_MAX_CLIENTS];
	//this->pfds[0].fd = this->serverSocket;
	//this->pfds[0].events = POLLIN;
}

void Server::newConnection()
{
	int	fd = 0;
	struct pollfd p;
	struct sockaddr_in address;
	
	socklen_t addrlen = sizeof(sockaddr);
	fd = accept(this->serverSocket, (struct sockaddr *)&address, &addrlen);
	setNonBlockingSocket(fd);
	std::string clientAddress(inet_ntoa(address.sin_addr));
	//Log::out("new connection with " + clientAddress + ":" + std::to_string(ntohs(this->addressServer.sin_port)));
	//this->pfds[this->numSockets].fd = fd;
	//this->pfds[this->numSockets].events = POLLIN | POLLOUT;

	p.fd = fd;
	p.events = POLLIN | POLLOUT;
	this->pfds.push_back(p);

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
