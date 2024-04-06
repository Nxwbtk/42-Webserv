#pragma once
#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>

class ConfigParser {
    public:
        ConfigParser();
        ConfigParser(const std::string& filename);
        ~ConfigParser();

        void parseConfig();
};
#endif