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
	this->numSockets++;
	return newSocket;
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
		for(nfds_t i = 0; i < this->numSockets; ++i)
		{
			if (this->pfds[i].revents & POLLIN)
			{
				ACmd *cmdTmp = 0;
				char buffer[MAX_MSG_SIZE];

				while (read(this->pfds[i].fd, buffer, MAX_MSG_SIZE))
					cmdTmp = this->clientList.find(this->pfds[i].fd)->second->updateCmd(buffer);
				if (cmdTmp)
					this->cmdList.push_back(cmdTmp);
			}
		}
		for (std::vector<ACmd *>::iterator it = this->cmdList.begin();
				it != this->cmdList.end(); ++it)
		{
			(*it)->execute(this->clientList);
		}
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
	close(this->serverSocket);
}

void Server::initPoll()
{
	this->pfds = new pollfd[SERV_MAX_CLIENTS]; 
	pfds[0].fd = this->serverSocket;
	pfds[0].events = POLLIN;
}

void Server::newConnection()
{
	int	fd = 0;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(sockaddr);

	fd = accept(this->serverSocket, (struct sockaddr *)&address, &addrlen);
	std::string clientAddress(inet_ntoa(address.sin_addr));
	Log::out("new connection with " + clientAddress + ":" + std::to_string(ntohs(this->addressServer.sin_port)));
	this->pfds[this->numSockets].fd = fd;
	this->pfds[this->numSockets].events = POLLIN | POLLOUT;
	this->clientList.insert(std::pair<int, Client*>(fd, new Client(clientAddress, fd)));
	this->clientList.find(fd)->second->addMsg("Welcome to IRC");
	this->numSockets++;
}
