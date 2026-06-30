#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "commands.hpp"

int main()
{
  pozdnyakov::Database db;

  using cmd_t = void (*)(std::istream &, std::ostream &, pozdnyakov::Database &);
  std::unordered_map< std::string, cmd_t > cmds = {
    {"note", pozdnyakov::cmdNote}, 
    {"line", pozdnyakov::cmdLine},       
    {"show", pozdnyakov::cmdShow},
    {"drop", pozdnyakov::cmdDrop}, 
    {"link", pozdnyakov::cmdLink},       
    {"mind", pozdnyakov::cmdMind},
    {"halt", pozdnyakov::cmdHalt}, 
    {"expired", pozdnyakov::cmdExpired}, 
    {"refresh", pozdnyakov::cmdRefresh}
  };

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, db);
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>\n";
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }

  return 0;
}
