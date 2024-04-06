#pragma once
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define NC "\033[0m"

#define BUFFER_SIZE 1024
#define MAX_EVENTS 1024
#define BACKLOG 100


/* CUSTOM LIBRARY */
# include "ConfigParser.hpp"
# include "Helpers.hpp"
# include "HttpHandler.hpp"
# include "./Server.hpp"

/* C++ LIBRARY */
# include <iostream>
# include <vector>
# include <set>
# include <map>
# include <csignal>
# include <ctime>
# include <exception>

/* C LIBRARY */
# include <sys/types.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>

/* NETWORK LIBRARY */
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <sys/epoll.h>

class Server;
class HttpHandler;

class Webserv {
	public:
		Webserv(std::string const& configFile = "");
		~Webserv();

	private:
		Webserv();

		int _backlog; // Maximum capacity of the queue for pending connections
		int	_epollFd; // File descriptor for epoll instance
		std::string _configFile; // Path to server configuration file
		std::vector<Server> _servers; // Collection of server instances
		std::set<int> _ports; // Unique set of ports assigned to servers to avoid duplicates
		std::set<int> _socketsFd; // Collection of socket file descriptors
		std::map<int, HttpHandler*> _contexts; // Mapping of socket descriptors to their HTTP request contexts
		std::map<int, int> _cgiList;

		void initializeServer();
		void loadConfiguration(const std::string& configFile);
		void setUpServerSockets();
		
		int runEventLoop();
		void processEvent(struct epoll_event& event);
		void handleIncomingEvent(struct epoll_event& event, HttpHandler* context);
		void handleOutgoingEvent(struct epoll_event& event, HttpHandler* context);
		void handleErrorEvent(struct epoll_event& event, HttpHandler* context);

		void cleanUpSockets();
		void cleanUpContexts();

		void epollAdd(int, int);
		void epollModify(int, int);
		void epollDelete(int);

		void closeConnection(int fd);
		void checkTimeouts();

};

#endif