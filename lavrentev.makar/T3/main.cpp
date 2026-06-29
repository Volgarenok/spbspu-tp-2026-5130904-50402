#include <iostream>
#include <fstream>
#include <map>
#include <limits>
#include "Figures.hpp"
#include "Area.hpp"
#include "Maxmin.hpp"
#include "Count.hpp"
#include "readfile.hpp"
#include "UserCommands.hpp"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Input processing error\n";
    return 1;
  }

  std::vector< lavrentev::Polygon > plgs;

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Input processing error\n";
    return 2;
  }

  lavrentev::readfile(file, plgs);

  using cmd_t = void (*)(std::istream &in, const std::vector< lavrentev::Polygon > &plgs);

  std::map< std::string, cmd_t > commands{};
  commands["AREA"] = lavrentev::area;
  commands["MAX"] = lavrentev::max;
  commands["MIN"] = lavrentev::min;
  commands["COUNT"] = lavrentev::count;
  commands["MAXSEQ"] = lavrentev::maxseq;
  commands["INTERSECTIONS"] = lavrentev::intersections;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      if (!commands.at(cmd))
      {
        throw std::logic_error("Unknown command");
      }
      commands[cmd](std::cin, plgs);
      std::cout << "\n";
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
