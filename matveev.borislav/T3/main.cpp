#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>

#include "commands.hpp"
#include "geometry.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  matveev::data_t polygons;
  using input_it_t = std::istream_iterator< matveev::Polygon >;

  while (!input.eof())
  {
    std::copy(input_it_t{ input }, input_it_t{}, std::back_inserter(polygons));

    if (input.fail() && !input.eof())
    {
      input.clear();
      input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  matveev::Contexts contexts(polygons);

  using handler_t = std::function< bool(std::istream&, std::ostream&, matveev::Contexts&) >;
  std::map< std::string, handler_t > commands;
  commands["AREA"] = matveev::doArea;
  commands["COUNT"] = matveev::doCount;
  commands["MAX"] = matveev::doMax;
  commands["MIN"] = matveev::doMin;
  commands["INFRAME"] = matveev::doInFrame;
  commands["INTERSECTIONS"] = matveev::doIntersections;
  commands["CONTEXT"] = matveev::doContext;
  commands["POPCONTEXT"] = matveev::doPopContext;
  commands["LEVEL"] = matveev::doLevel;

  std::string command;

  while (std::cin >> command)
  {
    try
    {
      if (commands.at(command)(std::cin, std::cout, contexts))
      {
        std::cout << "\n";
      }
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
    }

    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  return 0;
}
