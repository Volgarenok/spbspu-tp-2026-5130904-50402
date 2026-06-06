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
      {"note", pozdnyakov::cmdNote}, {"line", pozdnyakov::cmdLine},       {"show", pozdnyakov::cmdShow},
      {"drop", pozdnyakov::cmdDrop}, {"link", pozdnyakov::cmdLink},       {"mind", pozdnyakov::cmdMind},
      {"halt", pozdnyakov::cmdHalt}, {"expired", pozdnyakov::cmdExpired}, {"refresh", pozdnyakov::cmdRefresh}};

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      auto it = cmds.find(cmd);
      if (it == cmds.end()) {
        throw std::out_of_range("unknown command");
      }
      it->second(std::cin, std::cout, db);
    } catch (const std::out_of_range &) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    } catch (const std::logic_error &) {
      std::cout << "<INVALID COMMAND>\n";
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }

  return 0;
}
