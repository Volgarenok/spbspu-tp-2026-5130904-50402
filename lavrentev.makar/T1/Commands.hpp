#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <iomanip>

using cmd_t = void(*)(std::istream&, std::ostream&, std::vector< std::string >&);

#endif