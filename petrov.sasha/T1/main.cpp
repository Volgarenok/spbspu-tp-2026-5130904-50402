#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "note.hpp"

int main()
{
  petrov::notes_t db;
  const std::unordered_map< std::string, petrov::func_t > cmds{
      {"note", petrov::addNote},    {"line", petrov::addDesc},         {"show", petrov::printNote},
      {"drop", petrov::dropNote},   {"link", petrov::linkNote},        {"halt", petrov::removeLink},
      {"mind", petrov::printLinks}, {"expired", petrov::countExpired}, {"refresh", petrov::refreshLinks},
  };
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, db);
    } catch (const std::out_of_range& ) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(petrov::streamMax, '\n');
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

