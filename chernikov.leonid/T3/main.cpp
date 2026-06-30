#include "commands.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }

  std::vector< chernikov::Polygon > polygons;
  std::ifstream file(argv[1]);

  if (!file)
  {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }

  while (!file.eof())
  {
    std::copy(std::istream_iterator< chernikov::Polygon >(file), std::istream_iterator< chernikov::Polygon >(),
              std::back_inserter(polygons));
    if (!file)
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands;
  commands["AREA"] = std::bind(chernikov::area, std::placeholders::_1, std::placeholders::_2, std::cref(polygons));
  commands["MAX"] = std::bind(chernikov::max, std::placeholders::_1, std::placeholders::_2, std::cref(polygons));
  commands["MIN"] = std::bind(chernikov::min, std::placeholders::_1, std::placeholders::_2, std::cref(polygons));
  commands["COUNT"] = std::bind(chernikov::count, std::placeholders::_1, std::placeholders::_2, std::cref(polygons));
  commands["PERMS"] = std::bind(chernikov::perms, std::placeholders::_1, std::placeholders::_2, std::cref(polygons));
  commands["ECHO"] = std::bind(chernikov::echo, std::placeholders::_1, std::placeholders::_2, std::ref(polygons));
  commands["MAXSEQ"] = std::bind(chernikov::maxseq, std::placeholders::_1, std::placeholders::_2, std::cref(polygons));
  commands["RMECHO"] = std::bind(chernikov::rmecho, std::placeholders::_1, std::placeholders::_2, std::ref(polygons));
  commands["RECTS"] = std::bind(chernikov::rects, std::placeholders::_1, std::placeholders::_2, std::cref(polygons));
  commands["RIGHTSHAPES"] =
      std::bind(chernikov::rightshapes, std::placeholders::_1, std::placeholders::_2, std::cref(polygons));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)(std::cin, std::cout);
      std::cout << '\n';
    } catch (const std::exception &)
    {
      if (std::cin.fail())
      {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
