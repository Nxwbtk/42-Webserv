#include "HttpHandler.hpp"

HttpHandler::HttpHandler() {
}

HttpHandler::~HttpHandler() {
}

HttpHandler::HttpHandler(int connFd) : _connFd(connFd) {
}

void	HttpHandler::handle_request() {
}

bool	HttpHandler::ready_to_send() {
	return false;
}

void	HttpHandler::sending() {
}

bool	HttpHandler::should_close() {
	return false;
}

bool	HttpHandler::isTimedOut(time_t now) {
	return false;
}
