#include <fstream>
#include <iostream>
#include <limits>
#include <unordered_map>
#include "commands.hpp"
#include "parser.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Bad args\n";
    return 1;
  }

  std::ifstream in(argv[1]);
  if (!in.is_open())
  {
    std::cerr << "Couldn't open the file" << '\n';
    return 1;
  }

  shirokov::plg_t polygons = shirokov::parse(in);
  using cmd_t = void (*)(std::istream&, std::ostream&, shirokov::plg_t&);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["AREA"] = shirokov::area;
  cmds["MAX"] = shirokov::max;
  cmds["MIN"] = shirokov::min;
  cmds["COUNT"] = shirokov::count;
  cmds["MAXSEQ"] = shirokov::maxseq;
  cmds["RECTS"] = shirokov::rects;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, polygons);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toIgnore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toIgnore, '\n');
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
