#include <iostream>
#include <string>
#include <unordered_map>
#include <limits>

namespace novikov
{
  struct Note
  {
  };

  void note(std::istream& in, std::ostream&, std::unordered_map< std::string, novikov::Note > notes);
  void line(std::istream& in, std::ostream&, std::unordered_map< std::string, novikov::Note > notes);
  void show(std::istream& in, std::ostream& out, std::unordered_map< std::string, novikov::Note > notes);
  void drop(std::istream& in, std::ostream&, std::unordered_map< std::string, novikov::Note > notes);
  void link(std::istream& in, std::ostream&, std::unordered_map< std::string, novikov::Note > notes);
  void halt(std::istream& in, std::ostream&, std::unordered_map< std::string, novikov::Note > notes);
  void mind(std::istream& in, std::ostream& out, std::unordered_map< std::string, novikov::Note > notes);
  void expired(std::istream& in, std::ostream& out, std::unordered_map< std::string, novikov::Note > notes);
  void refresh(std::istream& in, std::ostream&, std::unordered_map< std::string, novikov::Note > notes);
}

int main()
{
  std::unordered_map< std::string, novikov::Note > notes;

  using cmd_t = void (*)(std::istream&, std::ostream&, std::unordered_map< std::string, novikov::Note >);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = novikov::note;
  cmds["line"] = novikov::line;
  cmds["show"] = novikov::show;
  cmds["drop"] = novikov::drop;
  cmds["link"] = novikov::link;
  cmds["halt"] = novikov::halt;
  cmds["mind"] = novikov::mind;
  cmds["expired"] = novikov::expired;
  cmds["refresh"] = novikov::refresh;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, notes);
    }
    catch (...)
    {
      std::cerr << "<INVALID COMMAND>\n";
      auto toIgnore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toIgnore, '\n');
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
