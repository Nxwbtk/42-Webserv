/* Simple C++ server to learn how to implement some basic functions
 * Compile: c++ -Wall -Wextra -Werror -std=c++98 simpleServer.cpp -o server
 * Run: ./server
 * Open a new terminal Run: nc localhost 4242 or curl http://localhost:4242 etc... */

#include <iostream>
#include <unistd.h> 
#include <cstdlib>
#include <cstring>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For the sockaddr_in struct

int	main() {
	// Create a socket (IPv4, TCP)
	int	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		std::cout << "Failed to create socket. errno: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Listen to port 4242 on any address
	sockaddr_in	sockaddr;
	
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(4242); // Converts host byte order to network byte order
	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0 ) {
		std::cout << "Failed to bind to port 4242. errno: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Start listening. Hold at most 10 connections in the queue
	if (listen(sockfd, 10) < 0) {
		std::cout << "Failed to listen on socket. errno: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Grab a connection from the queue
	size_t	addrlen = sizeof(sockaddr);
	int		connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
	if (connection < 0) {
		std::cout << "Failed to grab connection. errno: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Read from the connection
	char	buffer[100];
	ssize_t	bytesRead = read(connection, buffer, sizeof(buffer) - 1); // Read data into the buffer
	if (bytesRead < 0) {
		std::cout << "Failed to read data from socket. errno: " << strerror(errno) << std::endl;
		close(connection);
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	buffer[bytesRead] = '\0'; // // Null-terminate the received data
	std::cout << "The message was: " << buffer << std::endl;

	// Send a message to the connection
    std::string	response = "Hi there!\n";
	send(connection, response.c_str(), response.size(), 0);

	// Close the connections
	close(connection);
	close(sockfd);
}
