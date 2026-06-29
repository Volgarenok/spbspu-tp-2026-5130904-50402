#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include "operations.hpp"

int main()
{
  lachugin::notesMap db;
  std::unordered_map< std::string, lachugin::cmd_t > cmds;
  cmds["note"] = lachugin::addNote;
  cmds["line"] = lachugin::addLine;
  cmds["show"] = lachugin::showNote;
  cmds["drop"] = lachugin::removeNote;
  cmds["link"] = lachugin::addLink;
  cmds["mind"] = lachugin::printLinks;
  cmds["halt"] = lachugin::removeLink;
  cmds["expired"] = lachugin::allRemovedNotes;
  cmds["refresh"] = lachugin::linksRemover;
  cmds["loop"] = lachugin::loop;

  std::unordered_set< std::string > outputCmds =
  {
    "show",
    "mind",
    "expired",
    "loop"
  };

  std::string cmd;

  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, db);

      if (outputCmds.count(cmd))
      {
        std::cout << '\n';
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
