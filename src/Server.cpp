#include "Server.hpp"

Server::Server(const std::string &pPort, const std::string &pPass)
{
	this->pass = pPass;
	this->setPort(pPort);
	this->numSockets = 0;
	this->initServerSocket();
	this->initPoll();
}

void Server::setPort(const std::string &pPort)
{
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
}

void Server::initServerSocket()
{
	this->createServerSocket();
	this->setAddressServerStruct();
	this->runBind();
	this->runListen();
}

void Server::createServerSocket()
{
	int opt = 1;

	if ((this->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		Log::err("socket failed", 0);
		exit(1);
	}
	if (setsockopt(this->serverSocket, SOL_SOCKET,
				SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		Log::err("setsockopt failed", 0);
		exit(1);
	}
	setNonBlockingSocket(this->serverSocket);
	this->numSockets++;
}

void setNonBlockingSocket(const int &fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		Log::err("fcntl error: " + std::to_string(errno), 0);
}

void Server::setAddressServerStruct()
{
	this->addressServer.sin_family = AF_INET;
	if (!inet_aton(HOST, &(this->addressServer.sin_addr)))
	{
		Log::err("invalid IP this->addressServer", 0);
		exit(1);
	}
	this->addressServer.sin_port = htons(this->port);
}


void Server::runBind()
{
	if (bind(this->serverSocket, (struct sockaddr *) &this->addressServer, sizeof(this->addressServer)) < 0)
	{
		Log::err("bind failed", 0);
		exit(1);
	}
}

void Server::runListen()
{
	if (listen(serverSocket, 10) < 0)
	{
		Log::err("listen failed", 0);
		exit(1);
	}
}

void Server::initPoll()
{
	struct pollfd p;

	p.fd = this->serverSocket;
	p.events = POLLIN;
	this->pfds.push_back(p);
}

Server::~Server()
{
	for (nfds_t i = 0; i < this->numSockets; ++i)
		close(this->pfds[i].fd);
}

void Server::run()
{
	while (true)
	{
		this->runPoll();
		this->handleNewconnection();
		this->handlePauline();
		this->runCommands();
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
	setNonBlockingSocket(fd);
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
	std::string cmdTmp = "";
	
	for(nfds_t i = 1; i < this->numSockets; ++i)
	{
		if (this->pfds[i].revents & POLLIN)
		{
			cmdTmp = this->readUntilFlushSocket(this->pfds[i].fd);
			if (cmdTmp.length() != 0)
				this->cmdList.push_back(cmdFactory(cmdTmp, this->pfds[i].fd));
		}
	}
}

std::string Server::readUntilFlushSocket(const int &fd)
{
	std::string cmdTmp = "";
	char buffer[MAX_MSG_SIZE];
	int retVal;

	bzero(buffer, MAX_MSG_SIZE);
	retVal = recv(fd, buffer, MAX_MSG_SIZE, 0);
	while (retVal > 0)
	{
		cmdTmp = this->clientList.find(fd)->second->updateCmd(buffer);
		retVal = recv(fd, buffer, MAX_MSG_SIZE, 0);
	}
	if (this->isReadError(retVal))
		Log::err("read error for socket " + std::to_string(fd) + " errno: " + std::to_string(errno), 0);
	if (this->isConnectionClosed(retVal))
		this->closedPfdsIndex.push_back(fd);
	return cmdTmp;
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
	for (int i = 0; i < (int)this->closedPfdsIndex.size(); ++i)
	{
		for (unsigned long j = 0; j < this->pfds.size(); ++j)
		{
			if (this->pfds[j].fd == this->closedPfdsIndex[i])
			{
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

void Server::removeClient(const int &fd)
{
	delete this->clientList.find(fd)->second;
	this->clientList.erase(fd);
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
