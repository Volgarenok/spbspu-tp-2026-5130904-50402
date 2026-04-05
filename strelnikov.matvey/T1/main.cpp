#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include "functionality.hpp"

int main()
{
  const std::unordered_map< std::string, void (*)(std::istream &, std::ostream &, strelnikov::db_t &) > cmds{
      {"note", strelnikov::addNote},     {"line", strelnikov::addLine},    {"show", strelnikov::show},
      {"drop", strelnikov::drop},        {"link", strelnikov::link},       {"halt", strelnikov::halt},
      {"mind", strelnikov::showMindMap}, {"expired", strelnikov::expired}, {"refresh", strelnikov::refresh},
  };
  strelnikov::db_t db;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, db);
    } catch (const std::out_of_range &) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error &e) {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }

  if (!std::cin.eof()) {
    std::cerr << "Bad input!\n";
    return 1;
  }
  return 0;
}
