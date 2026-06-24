#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "note.hpp"

int main()
{
  petrov::notes_t db;
  const std::unordered_map< std::string, petrov::func_t > cmds{
      {"note", petrov::addNote},
      {"line", petrov::addDesc},
      {"show", petrov::printNote},
      {"drop", petrov::dropNote},
      {"link", petrov::linkNote},
      {"halt", petrov::removeLink},
      {"mind", petrov::printLinks},
      {"expired", petrov::countExpired},
      {"refresh", petrov::refreshLinks},
      {"loop", petrov::cmdLoop},
  };
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, db);
      if (cmd == "show" || cmd == "mind" || cmd == "expired" || cmd == "loop") {
        std::cout << '\n';
      }
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(petrov::streamMax, '\n');
    }
  }
}

