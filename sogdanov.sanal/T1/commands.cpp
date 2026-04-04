#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iomanip>
#include <stdexcept>
#include "commands.hpp"
namespace sogdanov
{
  using NotePtr = std::shared_ptr<Note>;
  using NoteMap = std::unordered_map<std::string, NotePtr>;
  void cmd_note(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string name;
    in >> name;
    auto it = notes.find(name);
    if (!(in >> name))
    {
      throw std::logic_error("No name");
    }
    if (it != notes.end() && it->second)
    {
      throw std::logic_error("Note already exists");
    }
    if (it == notes.end() || !it->second)
    {
      NotePtr n = std::make_shared<Note>();
      n->name = name;
      notes[name] = n;
    }
  }
  void cmd_line(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string name, text;
    if (!(in >> name >> std::quoted(text)))
    {
      throw std::logic_error("No line");
    }
    auto it = notes.find(name);
    if (it == notes.end() || !it->second)
    {
      throw std::logic_error("Note not found");
    }
    it->second->lines.push_back(text);
  }
  void cmd_show(std::istream &in, std::ostream &out, NoteMap &notes)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::logic_error("No name");
    }
    auto it = notes.find(name);
    if (it == notes.end() || !it->second)
    {
      throw std::logic_error("Note not found");
    }
    for (const std::string &l : it->second->lines)
    {
      out << l << "\n";
    }
  }
  void cmd_drop(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string name;
    in >> name;
    if (!(in >> name))
    {
      throw std::logic_error("No name");
    }
    auto it = notes.find(name);
    if (it == notes.end() || !it->second)
    {
      throw std::logic_error("note not found");
    }
    it->second.reset();
  }

}
