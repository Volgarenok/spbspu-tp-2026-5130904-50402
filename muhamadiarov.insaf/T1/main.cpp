#include <iostream>
#include <string>
#include <unordered_map>
#include <limits>
#include "action.hpp"

int main()
{
  namespace muh = muhamadiarov;
  using cmd_t = void(*)(std::istream &, std::ostream &, muh::NoteMap_t &);

  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = muh::note;
  cmds["line"] = muh::line;
  cmds["show"] = muh::show;
  cmds["drop"] = muh::drop;
  cmds["link"] = muh::link;
  cmds["halt"] = muh::halt;
  cmds["mind"] = muh::mind;
  cmds["expired"] = muh::expired;
  cmds["refresh"] = muh::refresh;

  muh::NoteMap_t result;
  std::string str;
  while (std::cin >> str)
  {
    try
    {
      cmds.at(str)(std::cin, std::cout, result);
    }
    catch (const std::exception&)
    {
      std::cerr << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
