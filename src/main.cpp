#include "Webserv.hpp"

/* Signal handler to safely exit on SIGINT */
static void signalHandler(int sig) {
	(void)sig; // Suppress unused parameter warning
	signal(SIGINT, SIG_IGN); // Ignore further SIGINT signals
	signal(SIGPIPE, SIG_IGN); // Ignore SIGPIPE to handle broken pipes
	throw(0); // Throw an exception to trigger clean shutdown
}

int main(int ac, char **av) {
	signal(SIGINT, signalHandler);

	try {
		if (ac == 1)
			Webserv server("");
		else if (ac == 2)
			Webserv server(av[1]);
		else {
			std::cerr << "Usage: ./webserv [config file]" << std::endl;
			return (EXIT_FAILURE);
		}
	} catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	} catch (...) {
		std::cerr << "An unknown error occurred." << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
