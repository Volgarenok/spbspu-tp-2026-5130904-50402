#include <iostream>
#include <string>
#include <unordered_map>
#include "command.hpp"
#include "note.hpp"

bool afanasev::has_output = false;

int main()
{
  afanasev::note_t db;

  const std::unordered_map< std::string, afanasev::func_t > cmds
  {
    {"note", afanasev::noteCmd},
    {"line", afanasev::lineCmd},
    {"show", afanasev::showCmd},
    {"drop", afanasev::dropCmd},

    {"link", afanasev::linkCmd},
    {"halt", afanasev::haltCmd},
    {"mind", afanasev::mindCmd},
    {"expired", afanasev::expiredCmd},
    {"refresh", afanasev::refreshCmd},
  };

  std::string cmd;

  while (std::cin >> cmd)
  {
    try
    {
      afanasev::has_output = false;
      cmds.at(cmd)(std::cin, std::cout, db);
      if (afanasev::has_output)
      {
        std::cout << '\n';
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(afanasev::streamMax, '\n');
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "Error input" << '\n';
    return 1;
  }

  return 0;
}
