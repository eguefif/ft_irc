#include "../includes/Server.hpp"

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
	if (!inet_aton("127.0.0.1", &(address.sin_addr)))
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
	struct sockaddr address;
	socklen_t addrlen = sizeof(sockaddr);

	fd = accept(this->serverSocket, (struct sockaddr *)&address, &addrlen);
	std::string str(inet_ntoa(this->addressServer.sin_addr));
	Log::out("new connection with " + str + ":" + std::to_string(ntohs(this->addressServer.sin_port)));
	this->pfds[this->numSockets].fd = fd;
	this->pfds[this->numSockets].events = POLLIN | POLLOUT;
	this->numSockets++;
}
