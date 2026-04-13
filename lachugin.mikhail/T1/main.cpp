#include "operations.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

int main() {
  lachugin::notesMap db;
  std::unordered_map< std::string, lachugin::cmd_t> cmds;
  cmds["note"] = lachugin::addNote;
  cmds["line"] = lachugin::addLine;
  cmds["show"] = lachugin::showNote;
  cmds["drop"] = lachugin::removeNote;
  cmds["link"] = lachugin::addLink;
  cmds["mind"] = lachugin::printLinks;
  cmds["halt"] = lachugin::removeLink;
  cmds["expired"] = lachugin::allRemovedNotes;
  cmds["refresh"] = lachugin::linksRemover;

  std::string cmd;

  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, db);
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (const std::logic_error& e)
    {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
