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

void alisov::line(std::istream &in, std::ostream &out, NotesMap &notes)
{
  std::string name;
  if (in >> name) {
    std::string text;
    if (in >> std::quoted(text)) {
      auto it = notes.find(name);
      if (it != notes.end()) {
        it->second->lines.push_back(text);
      }
    }
  }
}

void alisov::show(std::istream &in, std::ostream &out, NotesMap &notes)
{
  std::string name;
  if (in >> name) {
    auto it = notes.find(name);
    if (it != notes.end()) {
      for (const auto &str : it->second->lines) {
        out << str << "\n";
      }
    }
  }
}

void alisov::drop(std::istream &in, std::ostream &out, NotesMap &notes)
{
  std::string name;
  if (in >> name) {
    auto it = notes.find(name);
    if (it != notes.end()) {
      notes.erase(it);
    }
  }
}

void alisov::link(std::istream &in, std::ostream &out, NotesMap &notes)
{
  std::string from_name, to_name;
  if (in >> from_name >> to_name) {
    auto it_from = notes.find(from_name);
    auto it_to = notes.find(to_name);
    if (it_from != notes.end() && it_to != notes.end()) {
      bool exists = false;
      for (const auto &w_ptr : it_from->second->links) {
        if (auto s_ptr = w_ptr.lock()) {
          if (s_ptr == it_to->second) {
            exists = true;
            break;
          }
        }
      }
      if (!exists) {
        it_from->second->links.push_back(it_to->second);
      }
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
