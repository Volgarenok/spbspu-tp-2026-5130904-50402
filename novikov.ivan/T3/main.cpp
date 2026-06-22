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
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  std::ifstream in(argv[1]);
  if (!in.is_open())
  {
    std::cerr << "Could not open the file" << '\n';
    return 1;
  }

  novikov::plg_t polygons = novikov::parse(in);
  using cmd_t = void (*)(std::istream&, std::ostream&, novikov::plg_t&);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["AREA"] = novikov::area;
  cmds["MAX"] = novikov::max;
  cmds["MIN"] = novikov::min;
  cmds["COUNT"] = novikov::count;
  cmds["INTERSECTIONS"] = novikov::intersections;
  cmds["SAME"] = novikov::same;

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
    std::cerr << "Invalid input\n";
    return 1;
  }
}
