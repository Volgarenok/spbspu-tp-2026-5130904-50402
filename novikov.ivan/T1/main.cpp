#include <iostream>
#include <string>
#include <unordered_map>
#include <limits>
#include <memory>
#include <vector>

namespace novikov
{
  struct Note;
}

using map_t = std::unordered_map< std::string, std::shared_ptr< novikov::Note > >;

namespace novikov
{
  struct Note
  {
    std::vector< std::string > entries;
    std::vector< std::weak_ptr< Note > > links;
  };

  void note(std::istream& in, std::ostream&, map_t notes);
  void line(std::istream& in, std::ostream&, map_t notes);
  void show(std::istream& in, std::ostream& out, map_t notes);
  void drop(std::istream& in, std::ostream&, map_t notes);
  void link(std::istream& in, std::ostream&, map_t notes);
  void halt(std::istream& in, std::ostream&, map_t notes);
  void mind(std::istream& in, std::ostream& out, map_t notes);
  void expired(std::istream& in, std::ostream& out, map_t notes);
  void refresh(std::istream& in, std::ostream&, map_t notes);
}

int main()
{
  map_t notes;

  using cmd_t = void (*)(std::istream&, std::ostream&, map_t);
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
