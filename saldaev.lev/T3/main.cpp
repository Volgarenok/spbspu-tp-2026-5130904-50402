#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "commands.hpp"
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
  commands["AREA"] = std::bind(saldaev::handleArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(saldaev::handleMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(saldaev::handleMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(saldaev::handleCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["RECTS"] = std::bind(saldaev::handleRects, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAXSEQ"] = std::bind(saldaev::handleMexseq, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

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
