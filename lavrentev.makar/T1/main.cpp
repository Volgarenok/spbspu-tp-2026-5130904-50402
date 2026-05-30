#include <iostream>
#include <limits>
#include "Note.hpp"
#include "Commands.hpp"

int main()
{
  auto toignore = std::numeric_limits< std::streamsize >::max();
  std::cin.ignore(toignore, '\n');

  std::unordered_map<std::string, std::shared_ptr<lavrentev::Note>> db;
  std::unordered_map<std::string, lavrentev::cmd_t> cmds;

  cmds["note"] = lavrentev::note;
  cmds["line"] = lavrentev::line;
  cmds["show"] = lavrentev::show;
  cmds["drop"] = lavrentev::drop;
  cmds["link"] = lavrentev::link;
  cmds["halt"] = lavrentev::halt;
  cmds["mind"] = lavrentev::mind;
  cmds["expired"] = lavrentev::expired;
  cmds["refresh"] = lavrentev::refresh;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, db);
      if (cmd == "show")
      {
        std::cout << "\n";
      }
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
