#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "polygon_cmds.hpp"
#include "shapes.hpp"

int main(int argc, char **argv)
{
  namespace karp = karpovich;
  using iit_t = std::istream_iterator< karp::Polygon >;
  if (argc != 2) {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }
  std::vector< karp::Polygon > polygons;
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
  std::vector< karp::Polygon > currentContext = polygons;
  std::vector< std::vector< karp::Polygon > > contextStack;
  std::map< std::string, std::function< void() > > commands;
  auto rightshape = std::bind(karp::rightshapes, std::ref(std::cin), std::ref(std::cout), std::cref(currentContext));
  commands["AREA"] = std::bind(karp::area, std::ref(std::cin), std::ref(std::cout), std::cref(currentContext));
  commands["MAX"] = std::bind(karp::max, std::ref(std::cin), std::ref(std::cout), std::cref(currentContext));
  commands["MIN"] = std::bind(karp::min, std::ref(std::cin), std::ref(std::cout), std::cref(currentContext));
  commands["COUNT"] = std::bind(karp::count, std::ref(std::cin), std::ref(std::cout), std::cref(currentContext));
  commands["RIGHTSHAPES"] = rightshape;
  commands["SAME"] = std::bind(karp::same, std::ref(std::cin), std::ref(std::cout), std::cref(currentContext));
  commands["CONTEXT"] = std::bind(karp::context, std::ref(std::cin), std::ref(std::cout), std::ref(currentContext),
                                  std::ref(contextStack));
  commands["POPCONTEXT"] = std::bind(karp::popcontext, std::ref(std::cin), std::ref(std::cout), std::ref(contextStack));
  commands["LEVEL"] = std::bind(karp::level, std::ref(std::cin), std::ref(std::cout), std::cref(contextStack));

  std::string command;
  while (std::cin >> command) {
    try {
      commands.at(command)();
    } catch (const std::exception &) {
      if (std::cin.fail()) {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
