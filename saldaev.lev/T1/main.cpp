#include <limits>
#include <ostream>
#include "notes.hpp"

int main()
{
  saldaev::noteMap notes;
  std::unordered_map< std::string, saldaev::cmd_t > cmds;
  cmds["note"] = saldaev::handleNote;
  cmds["line"] = saldaev::handleLine;
  cmds["show"] = saldaev::handleShow;
  cmds["drop"] = saldaev::handleDrop;
  cmds["link"] = saldaev::handleLink;
  cmds["halt"] = saldaev::handleHalt;
  cmds["mind"] = saldaev::handleMind;
  cmds["expired"] = saldaev::handleExpired;
  cmds["refresh"] = saldaev::handleRefresh;

  std::string comand;
  while (std::cin >> comand) {
    try {
      cmds.at(comand)(std::cin, std::cout, notes);
      if (comand == "show" || comand == "expired" || comand == "mind") {
        std::cout << '\n';
      }
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      std::streamsize toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
}
