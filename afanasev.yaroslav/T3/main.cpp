#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <unordered_map>
#include <string>
#include <vector>
#include "commands.hpp"
#include "shape.hpp"

int main(int argc, char ** argv)
{
  namespace af = afanasev;
  using iit_t = std::istream_iterator< af::Polygon >;
  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();

  if (argc != 2)
  {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }

  std::vector< af::Polygon > shapes;

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }

  while (!file.eof())
  {
    std::copy(iit_t(file), iit_t(), std::back_inserter(shapes));
    if (!file)
    {
      file.clear();
      file.ignore(streamMax, '\n');
    }
  }

  std::unordered_map< std::string, std::function< void() > > commands;

  commands["AREA"]  = std::bind(af::area,  std::ref(std::cin), std::ref(std::cout), std::cref(shapes));
  commands["MAX"]   = std::bind(af::max,   std::ref(std::cin), std::ref(std::cout), std::cref(shapes));
  commands["MIN"]   = std::bind(af::min,   std::ref(std::cin), std::ref(std::cout), std::cref(shapes));
  commands["COUNT"] = std::bind(af::count, std::ref(std::cin), std::ref(std::cout), std::cref(shapes));

  commands["PERMS"] = std::bind(af::perms, std::ref(std::cin), std::ref(std::cout), std::cref(shapes));
  commands["RECTS"] = std::bind(af::rects, std::ref(std::cin), std::ref(std::cout), std::cref(shapes));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
      std::cout << "\n";
    }
    catch (const std::exception &)
    {
      if (std::cin.fail())
      {
        std::cin.clear();
      }

      std::cin.ignore(streamMax, '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
