#include <iostream>
#include <string>
#include <limits>
#include "op.hpp"

int main()
{
  novikov::mapNotes db;
  std::unordered_map< std::string, novikov::cmd_t > cmds;
  cmds["note"] = novikov::note;
  cmds["line"] = novikov::line;
  cmds["show"] = novikov::show;

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, db);
    } catch (const std::out_of_range &) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error &e) {
      std::cout << "<INVALID COMMAND " << e.what() << ">\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
  if (!std::cin.eof()) {
    return 1;
  }
}
