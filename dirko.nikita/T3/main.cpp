#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "cmds.hpp"

int main(int argc, char **argv)
{
  using iit_t = std::istream_iterator< dirko::Polygon >;
  if (argc != 2) {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }
  std::vector< dirko::Polygon > polygons;
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }
  while (!file.eof()) {
    std::copy(iit_t(file), iit_t(), std::back_inserter(polygons));
    if (!file) {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map< std::string, std::function< void() > > commands;
  commands["INFRAME"] = std::bind(dirko::inframe, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["RECTS"] = std::bind(dirko::rects, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["AREA"] = std::bind(dirko::area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(dirko::max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(dirko::min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(dirko::count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
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
