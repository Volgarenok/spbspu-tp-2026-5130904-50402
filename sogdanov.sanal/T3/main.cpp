#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "Commands.hpp"
#include "Polygon.hpp"

int main(int argc, char **argv)
{
  namespace sog = sogdanov;
  using iit_t = std::istream_iterator< sog::Polygon >;

  if (argc != 2) {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }

  std::vector< sog::Polygon > polygons;
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

  std::vector< std::vector< sog::Polygon > > contexts;
  contexts.push_back(polygons);

  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(sog::area, std::ref(std::cin), std::ref(std::cout), std::cref(contexts));
  commands["MAX"] = std::bind(sog::max, std::ref(std::cin), std::ref(std::cout), std::cref(contexts));
  commands["MIN"] = std::bind(sog::min, std::ref(std::cin), std::ref(std::cout), std::cref(contexts));
  commands["COUNT"] = std::bind(sog::count, std::ref(std::cin), std::ref(std::cout), std::cref(contexts));
  commands["PERMS"] = std::bind(sog::perms, std::ref(std::cin), std::ref(std::cout), std::cref(contexts));
  commands["MAXSEQ"] = std::bind(sog::maxseq, std::ref(std::cin), std::ref(std::cout), std::cref(contexts));
  
  commands["CONTEXT"] = std::bind(sog::context, std::ref(std::cin), std::ref(std::cout), std::ref(contexts));
  commands["POPCONTEXT"] = std::bind(sog::popcontext, std::ref(std::cin), std::ref(std::cout), std::ref(contexts));
  commands["LEVEL"] = std::bind(sog::level, std::ref(std::cin), std::ref(std::cout), std::cref(contexts));

  std::string command;
  while (std::cin >> command) {
    try {
      commands.at(command)();
      if ((command != "CONTEXT") && (command != "POPCONTEXT")) {
        std::cout << '\n';
      }
    } catch (const std::exception&) {
      if (std::cin.fail()) {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}