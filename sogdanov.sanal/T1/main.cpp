#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>

#include "commands.hpp"

int main()
{
  using cmd_t = void (*)(std::istream &, std::ostream &, sogdanov::NoteMap &);
  std::unordered_map< std::string, cmd_t > cmds;

  cmds["note"] = sogdanov::cmd_note;
  cmds["line"] = sogdanov::cmd_line;
  cmds["show"] = sogdanov::cmd_show;
  cmds["drop"] = sogdanov::cmd_drop;
  cmds["link"] = sogdanov::cmd_link;
  cmds["halt"] = sogdanov::cmd_halt;
  cmds["mind"] = sogdanov::cmd_mind;
  cmds["expired"] = sogdanov::cmd_expired;
  cmds["refresh"] = sogdanov::cmd_refresh;

  std::string cmd;
  sogdanov::NoteMap notes;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, notes);
    } catch (const std::logic_error &) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }

}
