#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <iostream>

using IniMap = std::map<std::string, std::map<std::string, std::string>>;
extern IniMap config;
IniMap readIni(const std::string& filename);