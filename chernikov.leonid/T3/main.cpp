#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include "commands.hpp"

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

  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::istringstream stream(line);
    std::string command;
    stream >> command;

    try
    {
      if (command == "AREA")
      {
        chernikov::area(stream, std::cout, polygons);
      } else if (command == "MAX")
      {
        chernikov::max(stream, std::cout, polygons);
      } else if (command == "MIN")
      {
        chernikov::min(stream, std::cout, polygons);
      } else if (command == "COUNT")
      {
        chernikov::count(stream, std::cout, polygons);
      } else if (command == "RECTS")
      {
        chernikov::rects(stream, std::cout, polygons);
      } else if (command == "RIGHTSHAPES")
      {
        chernikov::rightshapes(stream, std::cout, polygons);
      } else if (command == "PERMS")
      {
        chernikov::perms(stream, std::cout, polygons);
      } else if (command == "ECHO")
      {
        chernikov::echo(stream, std::cout, polygons);
      } else
      {
        std::cout << "<INVALID COMMAND>";
      }
      std::cout << '\n';
    } catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
