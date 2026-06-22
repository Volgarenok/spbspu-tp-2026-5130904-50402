#include <iostream>
#include "operations.hpp"
#include <unordered_map>

int main()
{
  std::unordered_map< std::string, void (*)(std::istream &, std::ostream &, karpenkov::mapOfNotes &) > listOfCommands{
      {"note", karpenkov::addNote},   {"line", karpenkov::addLine},        {"show", karpenkov::showNote},
      {"drop", karpenkov::dropNote},  {"link", karpenkov::addLink},        {"halt", karpenkov::removeLink},
      {"mind", karpenkov::showLinks}, {"expired", karpenkov::showExpired}, {"refresh", karpenkov::refreshLinks}};
  karpenkov::mapOfNotes notes;
  std::string command;
  while (std::cin >> command) {
    try {
      listOfCommands.at(command)(std::cin, std::cout, notes);
    } catch (...) {
      std::cout << "<INVALID COMMAND>";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
