#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include "note.hpp"

int main()
{
  karpovich::NoteMap notes;
  using cmd_t = void (*)(std::istream &, std::ostream &, karpovich::NoteMap &);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = karpovich::cmdNote;
  cmds["line"] = karpovich::cmdLine;
  cmds["show"] = karpovich::cmdShow;
  cmds["drop"] = karpovich::cmdDrop;
  cmds["link"] = karpovich::cmdLink;
  cmds["halt"] = karpovich::cmdHalt;
  cmds["mind"] = karpovich::cmdMind;
  cmds["expired"] = karpovich::cmdExpired;
  cmds["refresh"] = karpovich::cmdRefresh;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, notes);
      std::cout << '\n';
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
