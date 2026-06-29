#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include "commands.hpp"

int main()
{
  matveev::db_t db;
  matveev::cmd_map_t cmds;
  cmds["note"] = matveev::create_note;
  cmds["line"] = matveev::add_line;
  cmds["show"] = matveev::show_note;
  cmds["drop"] = matveev::drop_note;
  cmds["link"] = matveev::link_note;
  cmds["mind"] = matveev::mind_note;
  cmds["halt"] = matveev::halt_note;
  cmds["expired"] = matveev::expired_note;
  cmds["refresh"] = matveev::refresh_note;
  cmds["loop"] = matveev::loop_note;
  const std::set< std::string > printing = { "show", "mind", "expired", "loop" };
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, db);
      if (printing.count(cmd) != 0)
      {
        std::cout << "\n";
      }
    }
    catch (const std::logic_error&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
