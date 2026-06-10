#include "note.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include <memory>

using d_t = std::unordered_map< std::string, std::shared_ptr< khalikov::Note > >;

int main()
{
  d_t data;
  using cmd_t = void (*)(std::istream &, std::ostream &, d_t &);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = khalikov::noteCommand;
  cmds["line"] = khalikov::lineCommand;
  cmds["show"] = khalikov::showCommand;
  cmds["drop"] = khalikov::dropCommand;
  cmds["link"] = khalikov::linkCommand;
  cmds["halt"] = khalikov::haltCommand;
  cmds["mind"] = khalikov::mindCommand;
  cmds["expired"] = khalikov::expiredCommand;
  cmds["refresh"] = khalikov::refreshCommand;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, data);
      if (cmd == "show" || cmd == "mind") {
        std::cout << '\n';
      }
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}
