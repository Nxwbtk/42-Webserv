#pragma once
#ifndef HTTPHANDLER_HPP
# define HTTPHANDLER_HPP

# include <ctime>

class HttpHandler {
	private:
		int _connFd;
	public:
		HttpHandler();
		HttpHandler(int connFd);
		~HttpHandler();
		void handle_request();
		bool ready_to_send();
		void sending();
		bool should_close();
		bool isTimedOut(time_t now);
};

#endif