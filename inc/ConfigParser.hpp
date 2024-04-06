#pragma once
#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include <vector>
# include "Server.hpp"

class Server;

class ConfigParser {
    private:
        std::string _filename;
    public:
        ConfigParser();
        ConfigParser(const std::string& filename);
        ~ConfigParser();

        void parseConfig(std::vector<Server> _server);
};
#endif