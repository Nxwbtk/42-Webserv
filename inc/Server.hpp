#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

class Server {
	public:
		Server();
		~Server();
		int getPort();
		void initializeSocket(int backlog);
		int getSocketFd();
};

#endif