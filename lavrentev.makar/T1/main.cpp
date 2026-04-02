#include "Note.hpp"
#include "Commands.hpp"
#include <iostream>
#include <limits>

int main()
{
  std::unordered_map< std::string, std::shared_ptr< lavrentev::Note > > db;
  //using cmd_t = void(*)(std::istream&, std::ostream&, std::vector< std::string >&);
  std::unordered_map< std::string, cmd_t > cmds;

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
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, db);
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }

  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}
