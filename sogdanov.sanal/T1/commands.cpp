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
  void cmd_link(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string from, to;
    if (!(in >> from >> to))
    {
      throw std::logic_error("No arguments");
    }
    auto it_from = notes.find(from);
    auto it_to = notes.find(to);
    if (it_from == notes.end() || !it_from->second)
    {
      throw std::logic_error("note not found");
    }
    if (it_to == notes.end() || !it_to->second)
    {
      throw std::logic_error("note not found");
    }
    NotePtr note_from = it_from->second;
    NotePtr note_to = it_to->second;
    for (const std::weak_ptr<Note> &w : note_from->links)
    {
      if (w.lock() == note_to)
      {
        throw std::logic_error("link already exists");
      }
    }
    note_from->links.push_back(note_to);
  }
  void cmd_halt(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string from, to;
    if (!(in >> from >> to))
    {
      throw std::logic_error("no arguments");
    }
    auto it_from = notes.find(from);
    if (it_from == notes.end() || !it_from->second)
    {
      throw std::logic_error("note not found");
    }
    NotePtr note_from = it_from->second;
    std::vector<std::weak_ptr<Note>> &links = note_from->links;
    for (auto it = links.begin(); it != links.end(); ++it)
    {
      NotePtr target = it->lock();
      if (target && target->name == to)
      {
        links.erase(it);
        return;
      }
    }
    throw std::logic_error("link not found");
  }
  void cmd_mind(std::istream &in, std::ostream &out, NoteMap &notes)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::logic_error("no name");
    }
    auto it = notes.find(name);
    if (it == notes.end() || !it->second)
    {
      throw std::logic_error("note not found");
    }
    for (const std::weak_ptr<Note> &w : it->second->links)
    {
      NotePtr target = w.lock();
      if (target)
      {
        out << target->name << "\n";
      }
    }
  }
  void cmd_expired(std::istream &in, std::ostream &out, NoteMap &notes)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::logic_error("no name");
    }
    auto it = notes.find(name);
    if (it == notes.end() || !it->second)
    {
      throw std::logic_error("note not found");
    }
    int count = 0;
    for (const std::weak_ptr<Note> &w : it->second->links)
    {
      if (w.expired())
      {
        ++count;
      }
    }
    out << count << "\n";
  }
  void cmd_refresh(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::logic_error("no name");
    }
    auto it = notes.find(name);
    if (it == notes.end() || !it->second)
    {
      throw std::logic_error("note not found");
    }
    std::vector<std::weak_ptr<Note>> &links = it->second->links;
    for (auto i = links.begin(); i != links.end();)
    {
      if (i->expired())
      {
        i = links.erase(i);
      }
      else
      {
        ++i;
      }
    }
  }
}
