#include "Note.hpp"
#include "Commands.hpp"
#include <iostream>

int main()
{
  std::vector< lavrentev::Note > db;
  using cmd_t = void(*)(std::istream&, std::ostream&, std::vector< std::string >&);
  std::unordered_map< std::string, cmd_t > cmds;

  cmds["add"] = lavrentev::add_string;
  cmds["show-last"] = lavrentev::show_last;
  cmds["hello"] = lavrentev::hi;
  cmds["noop"] = lavrentev::noop;

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      //cmds.at(cmd)(std::cin, std::cout, db);
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
