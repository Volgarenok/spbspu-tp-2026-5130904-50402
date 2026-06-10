#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <limits>
#include <memory>
#include "commands.hpp"

int main()
{
  novikov::map_t notes;

  using cmd_t = void (*)(std::istream&, std::ostream&, novikov::map_t&);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = novikov::note;
  cmds["line"] = novikov::line;
  cmds["show"] = novikov::show;
  cmds["drop"] = novikov::drop;
  cmds["link"] = novikov::link;
  cmds["halt"] = novikov::halt;
  cmds["mind"] = novikov::mind;
  cmds["expired"] = novikov::expired;
  cmds["refresh"] = novikov::refresh;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, notes);
      if (cmd == "show" || cmd == "mind" || cmd == "expired")
      {
        std::cout << "\n";
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
