#include <ios>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <string>
#include <unordered_map>
#include "commands.hpp"

int main()
{
  shirokov::map_t notes;

  using cmd_t = void (*)(std::istream&, std::ostream&, shirokov::map_t&);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = shirokov::note;
  cmds["line"] = shirokov::line;
  cmds["show"] = shirokov::show;
  cmds["drop"] = shirokov::drop;
  cmds["link"] = shirokov::link;
  cmds["halt"] = shirokov::halt;
  cmds["mind"] = shirokov::mind;
  cmds["expired"] = shirokov::expired;
  cmds["refresh"] = shirokov::refresh;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, notes);
      if (cmd == "show" || cmd == "mind" || cmd == "expired")
      {
        std::cout << '\n';
      }
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
