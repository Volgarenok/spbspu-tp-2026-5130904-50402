#include "Note.hpp"

int main()
{
  alisov::NotesMap notes;
  using cmd1 = void (*)(std::istream &, std::ostream &, alisov::NotesMap &);
  std::unordered_map< std::string, cmd1 > cmds;

  cmds["note"] = alisov::note;
  cmds["line"] = alisov::line;
  cmds["show"] = alisov::show;
  cmds["drop"] = alisov::drop;
  cmds["link"] = alisov::link;
  cmds["halt"] = alisov::halt;
  cmds["mind"] = alisov::mind;
  cmds["expired"] = alisov::expired;
  cmds["refresh"] = alisov::refresh;

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, notes);
    } catch (const std::out_of_range &) {
      std::cout << "Invalid command\n";
      std::string dummy;
      std::getline(std::cin, dummy);
    }
  }
}
