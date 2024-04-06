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
