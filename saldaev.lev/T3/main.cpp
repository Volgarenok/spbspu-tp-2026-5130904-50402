#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "polygon.hpp"

int main(int argc, char **argv)
{
  using iit_t = std::istream_iterator< saldaev::Polygon >;
  if (argc != 2) {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }

  std::vector< saldaev::Polygon > polygons;

  while (!file.eof()) {
    std::copy(iit_t(file), iit_t(), std::back_inserter(polygons));
    if (!file) {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > commands;

  std::string command;
  while (std::cin >> command) {
    try {
      commands.at(command)();
      std::cout << '\n';
    } catch (const std::exception &) {
      if (std::cin.fail()) {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
