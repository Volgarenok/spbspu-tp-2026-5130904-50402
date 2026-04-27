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
  cmds["note"] = note;
  cmds["line"] = line;
  cmds["show"] = show;
  cmds["drop"] = drop;
  cmds["link"] = link;
  cmds["halt"] = halt;
  cmds["mind"] = mind;
  cmds["expired"] = expired;
  cmds["refresh"] = refresh;

  muh::NoteMap_t result;
  std::string str;
  while (std::cin >> str)
  {
    try
    {
      cmds.at(str)(std::cin, std::cout, result);
    }
    catch (const std::exception &)
    {
      std::cerr << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), \n);
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "<BAD INPUT>\n";
    return 1;
  }
  return 0;
}
