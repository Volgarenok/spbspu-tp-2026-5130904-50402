#include "Polygon.hpp"
#include "Commands.hpp"
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <functional>
#include <limits>
#include <string>

int main(int argc, char **argv)
{
  using iit_t = std::istream_iterator< sogdanov::Polygon >;

  if (argc != 2) {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }

  std::vector< sogdanov::Polygon > polygons;
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }

  while (!file.eof()) {
    std::copy(iit_t(file), iit_t(), std::back_inserter(polygons));
    if (file.fail() && !file.eof()) {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(sogdanov::area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(sogdanov::max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(sogdanov::min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(sogdanov::count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["PERMS"] = std::bind(sogdanov::perms, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAXSEQ"] = std::bind(sogdanov::maxseq, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::string command;
  while (std::cin >> command) {
    try {
      commands.at(command)();
    } catch (const std::exception&) {
      if (std::cin.fail()) {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

}
