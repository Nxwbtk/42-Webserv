#include "Webserv.hpp"

/* Default constructor */
Webserv::Webserv() : _backlog(BACKLOG), _configFile("") {
	initializeServer();
}

/* Paramater constructor with configuration file */
Webserv::Webserv(const std::string& configFile) : _backlog(BACKLOG), _configFile(configFile) {
	initializeServer();
}

/* Destructor to clean up resources */
Webserv::~Webserv() {
	cleanUpSockets();
	cleanUpContexts();
	close(_epollFd);
	std::cout << MAGENTA "Server shutdown complete." << NC << std::endl;
}

/* Initialize, start, and run the server */
void Webserv::initializeServer() {
	std::cout << GREEN "Server is starting..." << NC << std::endl;

	try {
		loadConfiguration(_configFile);
		setUpServerSockets();
		runEventLoop();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
	} catch (int code) {
		if (code == 0)
			std::cout << YELLOW "Shutdown signal received. Shutting down server..." << NC << std::endl;
		else
			std::cerr << RED "Unexpected shutdown. Exiting..." << NC << std::endl;
	}
}

/* Load server configuration from file */
void Webserv::loadConfiguration(const std::string& filename) {
	if (filename.empty()) {
		ConfigParser defaultConfig;
		defaultConfig.parseConfig(_servers); // I need a parsing method from the ConfigParser class
	} else {
		ConfigParser configFile(filename);
		configFile.parseConfig(_servers); // I need a parsing method from the ConfigParser class
	}
}

/* Set up server sockets according to the loaded configuration */
void Webserv::setUpServerSockets() {
	std::cout << CYAN "Setting up server sockets..." << NC << std::endl;
	
	std::vector<Server>::iterator it = _servers.begin();

	for (; it != _servers.end(); ++it) {
		int port = it->getPort(); // I need a getter from the Server class
		if (_ports.find(port) == _ports.end()) {
			it->initializeSocket(BACKLOG); // I need an initializeSocket method from the Server class
			_socketsFd.insert(it->getSocketFd()); // I need a getter from the Server class
			_ports.insert(port);
		} else {
			throw std::runtime_error("Port " + std::to_string(port) + " is duplicated.");
		}
	}
}

// Clean up server sockets
void Webserv::cleanUpSockets() {
	std::set<int>::iterator it = _socketsFd.begin();

	for (; it != _socketsFd.end(); ++it) {
		if (_contexts.find(*it) != _contexts.end()) {
			epollDelete(*it);
			close(*it);
		}
	}
	_socketsFd.clear();
}

// Clean up request handler contexts
void Webserv::cleanUpContexts() {
	std::map<int, HttpHandler*>::iterator it = _contexts.begin();

	for (; it != _contexts.end(); ++it) {
		epollDelete(it->first);
		close(it->first);

		if (_cgiList.find(it->first) != _cgiList.end()) {
			if (it->second != NULL) {
				delete it->second;
				it->second = NULL;
			}
		}
	}
}
