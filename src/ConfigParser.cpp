#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {
    // Default constructor
}

ConfigParser::ConfigParser(const std::string& filename) {
    // Parameter constructor
    _filename = filename;
}

ConfigParser::~ConfigParser() {
    // Destructor
}

void ConfigParser::parseConfig(std::vector<Server> _server) {
    // Parse the configuration file
    (void)_server;
    // I need to implement this method
}
