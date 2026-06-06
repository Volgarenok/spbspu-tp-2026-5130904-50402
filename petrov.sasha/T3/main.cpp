#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "commands.hpp"
#include "shapes.hpp"

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }
  std::vector< petrov::Polygon > polygons;
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }

  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();
  using iit_t = std::istream_iterator< petrov::Polygon >;

  while (!file.eof()) {
    std::copy(iit_t(file), iit_t(), std::back_inserter(polygons));
    if (!file) {
      file.clear();
      file.ignore(streamMax, '\n');
    }
  }

  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(petrov::area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(petrov::max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(petrov::min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(petrov::count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["PERMS"] = std::bind(petrov::perms, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["INFRAME"] = std::bind(petrov::inframe, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::string command;
  while (std::cin >> command) {
    try {
      commands.at(command)();
    } catch (const std::exception&) {
      if (std::cin.fail()) {
        std::cin.clear();
      }
      std::cin.ignore(streamMax, '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
