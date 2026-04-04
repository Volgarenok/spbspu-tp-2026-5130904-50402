#include <iostream>
#include <string>
#include <ostream>
#include <string>
#include <unordered_map>

namespace alisov
{
  struct Note
  {
  };

  void note(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void line(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void show(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void drop(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void link(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void halt(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void mind(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void expired(std::istream &in, std::ostream &out,
               std::unordered_map< std::string, alisov::Note > notes);
  void refresh(std::istream &in, std::ostream &out,
               std::unordered_map< std::string, alisov::Note > notes);
}

int main()
{
  std::unordered_map< std::string, alisov::Note > notes;
  using cmd1 =
      void (*)(std::istream &, std::ostream &, std::unordered_map< std::string, alisov::Note >);
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
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, notes);
    }
    catch (std::out_of_range &e)
    {
      std::cout << "Invalid command\n";
    }
    if (!std::cin.eof())
    {
      std::cerr << "Bad input\n";
      return 1;
    }
  }
}