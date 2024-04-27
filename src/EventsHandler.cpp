#include "Webserv.hpp"

int Webserv::runEventLoop() {
	// Create epoll instance
	_epollFd = epoll_create(1);

	if (_epollFd == -1) {
		std::cerr << RED "Error: epoll_create() failed." NC << std::endl;
		throw -1;
	}

	// Add listener sockets to epoll list
	struct epoll_event ev;
	std::set<int>::iterator it;
	for (it = _socketsFd.begin(); it != _socketsFd.end(); ++it)
	{
		ev.events = EPOLLIN;
		ev.data.fd = *it;
		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, *it, &ev) == -1) {
			std::cerr << RED "Error: Failed to add listener to epoll list." NC << std::endl;
			throw -1;
		}
		std::cout << GREEN "Listener [" << *it << "] added to epoll list." NC << std::endl;
	}

	std::cout << GREEN "Entering epoll loop." << NC << std::endl;

	while (true)
	{
		struct epoll_event events[MAX_EVENTS];
		int nFds = epoll_wait(_epollFd, events, MAX_EVENTS, 1000);
		checkTimeouts();

		if (nFds == -1)
		{
			if (errno == EINTR)
			{
				std::cout << "Warning: epoll_wait interrupted. Continuing..." << std::endl;
				continue;
			}
			std::cerr << "Error: epoll_wait fatal error. Terminating..." << std::endl;
			throw -1;
		}

		for (int i = 0; i < nFds; ++i) {
			processEvent(events[i]);
		}
	}

	std::cout << "Exited epoll loop." << std::endl;
	return 0;
}

void Webserv::processEvent(struct epoll_event& event) {
	HttpHandler* context = NULL;

	if (_contexts.find(event.data.fd) != _contexts.end())
		context = _contexts[event.data.fd];

	if (event.events & EPOLLIN) {
		handleIncomingEvent(event, context);
	}
	else if (event.events & EPOLLOUT) {
		handleOutgoingEvent(event, context);
	}
	else if (event.events & (EPOLLHUP | EPOLLERR)) {
		handleErrorEvent(event, context);
	}
}

void Webserv::handleIncomingEvent(struct epoll_event& event, HttpHandler* context) {
	if (_socketsFd.find(event.data.fd) != _socketsFd.end()) {
		// Accept new connection
		struct sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);
		int connFd = accept(event.data.fd, (struct sockaddr*)&clientAddr, &clientAddrLen);

		if (connFd == -1) {
			std::cerr << "Error: Failed to accept new connection." << std::endl;
			return;
		}
		// Make the socket non-blocking
		int flags = fcntl(connFd, F_GETFL, 0);
		if (flags == -1) flags = 0;
		fcntl(connFd, F_SETFL, flags | O_NONBLOCK);

		epollAdd(connFd, EPOLLIN); // Add new connection to epoll instance for monitoring
		_contexts[connFd] = new HttpHandler(connFd); // Create a new context for this connection
		std::cout << "New connection [" << connFd << "] accepted." << std::endl;
	} else if (context != NULL) {
		// Handle read event for existing connection
		context->handle_request(); // I need a handle_request method in HttpHandler
		// After handling the request, if we need to write a response, modify the epoll interest list
		if (context->ready_to_send()) { // I need a ready_to_send method in HttpHandler
			epollModify(event.data.fd, EPOLLOUT);
		}
	}
}

void Webserv::handleOutgoingEvent(struct epoll_event& event, HttpHandler* context) {
	if (context != NULL) {
		context->sending(); // I need a sending method in HttpHandler
		// Check if the connection should be closed or switched back to listening for read events
		if (context->should_close()) { // I need a should_close method in HttpHandler
			closeConnection(event.data.fd);
		} else {
			epollModify(event.data.fd, EPOLLIN); // Switch back to listening for read events if keep-alive
		}
	}
}

void Webserv::handleErrorEvent(struct epoll_event& event, HttpHandler* context) {
	std::cerr << "Error or hang-up on fd [" << event.data.fd << "]." << std::endl;
	closeConnection(event.data.fd);
}

void Webserv::closeConnection(int fd) {
	if (_contexts.find(fd) != _contexts.end()) {
		delete _contexts[fd];
		_contexts.erase(fd);
	}
	epollDelete(fd);
	close(fd);
	std::cout << "Connection [" << fd << "] closed and cleaned up." << std::endl;
}

void Webserv::checkTimeouts() {
	time_t now;
	time(&now);
	std::vector<int> toClose;

	for (std::map<int, HttpHandler*>::iterator it = _contexts.begin(); it != _contexts.end(); ++it) {
		if (it->second->isTimedOut(now)) { // I need an isTimedOut method in HttpHandler
			toClose.push_back(it->first);
		}
	}

	for (std::vector<int>::iterator it = toClose.begin(); it != toClose.end(); ++it) {
		std::cout << "Connection [" << *it << "] timed out." << std::endl;
		closeConnection(*it);
	}
}

void Webserv::epollAdd(int fd, int op) {
	struct epoll_event ev;
	ev.events = op;
	ev.data.fd = fd;

	if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		std::cerr << "Error: Failed to add fd to epoll." << std::endl;
		throw -1;
	}
}

void Webserv::epollModify(int fd, int op) {
	struct epoll_event ev;
	ev.events = op;
	ev.data.fd = fd;

	if (epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, fd, &ev) == -1) {
		std::cerr << "Error: Failed to modify fd in epoll." << std::endl;
		throw -1;
	}
}

void Webserv::epollDelete(int fd) {
	if (epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		std::cerr << "Error: Failed to delete fd from epoll." << std::endl;
		throw -1;
	}
}
