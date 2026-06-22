#include <iostream>
#include "operations.hpp"
#include <unordered_map>
#include <sstream>
#include <limits>

int main()
{
  std::unordered_map< std::string, void (*)(std::istream &, std::ostream &, karpenkov::mapOfNotes &) > listOfCommands{
      {"note", karpenkov::addNote},   {"line", karpenkov::addLine},        {"show", karpenkov::showNote},
      {"drop", karpenkov::dropNote},  {"link", karpenkov::addLink},        {"halt", karpenkov::removeLink},
      {"mind", karpenkov::showLinks}, {"expired", karpenkov::showExpired}, {"refresh", karpenkov::refreshLinks}};
  karpenkov::mapOfNotes notes;
  std::string line;

  while (std::getline(std::cin, line)) {
    std::istringstream in(line);
    std::string command;
    if (!(in >> command)) {
      continue;
    }
    try {
      listOfCommands.at(command)(in, std::cout, notes);
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
