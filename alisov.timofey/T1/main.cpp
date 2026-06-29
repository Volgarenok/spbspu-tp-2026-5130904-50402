#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace alisov
{
  struct Note
  {
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > links;
  };

  using NotesMap = std::unordered_map< std::string, std::shared_ptr< alisov::Note > >;

  void note(std::istream &in, std::ostream &out, NotesMap &notes);
  void line(std::istream &in, std::ostream &out, NotesMap &notes);
  void show(std::istream &in, std::ostream &out, NotesMap &notes);
  void drop(std::istream &in, std::ostream &out, NotesMap &notes);
  void link(std::istream &in, std::ostream &out, NotesMap &notes);
  void halt(std::istream &in, std::ostream &out, NotesMap &notes);
  void mind(std::istream &in, std::ostream &out, NotesMap &notes);
  void expired(std::istream &in, std::ostream &out, NotesMap &notes);
  void refresh(std::istream &in, std::ostream &out, NotesMap &notes);
}

void alisov::line(std::istream &, std::ostream &, NotesMap &)
{}
void alisov::show(std::istream &, std::ostream &, NotesMap &)
{}
void alisov::drop(std::istream &, std::ostream &, NotesMap &)
{}
void alisov::link(std::istream &, std::ostream &, NotesMap &)
{}
void alisov::halt(std::istream &, std::ostream &, NotesMap &)
{}
void alisov::mind(std::istream &, std::ostream &, NotesMap &)
{}
void alisov::expired(std::istream &, std::ostream &, NotesMap &)
{}
void alisov::refresh(std::istream &, std::ostream &, NotesMap &)
{}

void alisov::note(std::istream &in, std::ostream &out, NotesMap &notes)
{
  std::string name;
  if (in >> name) {
    if (notes.find(name) == notes.end()) {
      notes[name] = std::make_shared< alisov::Note >();
    }
  }
}

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
