#include <iostream>
#include <string>
#include <unordered_map>
#include "note.hpp"

int main()
{
  dirko::notes_t db;
  const std::unordered_map< std::string, dirko::func_t > cmds{
      {"note", dirko::addNote},    {"line", dirko::addDesc},         {"show", dirko::printNote},
      {"drop", dirko::dropNote},   {"link", dirko::linkNote},        {"halt", dirko::removeLink},
      {"mind", dirko::printLinks}, {"expired", dirko::countExpired}, {"refresh", dirko::refreshLinks},
  };
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, db);
      if (cmd == "show" || cmd == "mind" || cmd == "expired") {
        std::cout << '\n';
      }
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(dirko::streamMax, '\n');
    }
  }
}
