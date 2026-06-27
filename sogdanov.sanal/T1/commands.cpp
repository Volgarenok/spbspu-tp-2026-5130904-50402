#include "commands.hpp"

#include <iomanip>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

void sogdanov::cmd_note(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  if (notes.find(name) != notes.end()) {
    throw std::logic_error("note already exists");
  }
  notes.emplace(name, std::make_shared< Note >(name));
}

void sogdanov::cmd_line(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  std::string text;
  if (!(in >> name >> std::quoted(text))) {
    throw std::logic_error("no arguments");
  }
  notes.at(name)->lines.push_back(text);
}

void sogdanov::cmd_show(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  NotePtr note = notes.at(name);
  if (!note->lines.empty()) {
    out << note->lines[0];
    for (size_t i = 1; i < note->lines.size(); ++i) {
      out << "\n" << note->lines[i];
    }
  }
}

void sogdanov::cmd_drop(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  notes.at(name);
  notes.erase(name);
}

void sogdanov::cmd_link(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string from;
  std::string to;
  if (!(in >> from >> to)) {
    throw std::logic_error("no arguments");
  }
  NotePtr note_from = notes.at(from);
  NotePtr note_to = notes.at(to);
  for (const std::weak_ptr< Note > &w : note_from->links) {
    if (w.lock() == note_to) {
      throw std::logic_error("link already exists");
    }
  }
  note_from->links.push_back(note_to);
}

void sogdanov::cmd_halt(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string from;
  std::string to;
  if (!(in >> from >> to)) {
    throw std::logic_error("no arguments");
  }
  NotePtr note_from = notes.at(from);
  std::vector< std::weak_ptr< Note > > &links = note_from->links;
  for (auto it = links.begin(); it != links.end(); ++it) {
    NotePtr target = it->lock();
    if (target && target->name == to) {
      links.erase(it);
      return;
    }
  }
  throw std::logic_error("link not found");
}

void sogdanov::cmd_mind(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  NotePtr note = notes.at(name);
  bool first = true;
  for (const std::weak_ptr< Note > &w : note->links) {
    NotePtr target = w.lock();
    if (target) {
      if (!first) {
        out << "\n";
      }
      out << target->name;
      first = false;
    }
  }
}

void sogdanov::cmd_expired(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  NotePtr note = notes.at(name);
  int count = 0;
  for (const std::weak_ptr< Note > &w : note->links) {
    if (w.expired()) {
      ++count;
    }
  }
  out << count;
}

void sogdanov::cmd_refresh(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  NotePtr note = notes.at(name);
  std::vector< std::weak_ptr< Note > > &links = note->links;
  for (auto i = links.begin(); i != links.end();) {
    if (i->expired()) {
      i = links.erase(i);
    } else {
      ++i;
    }
  }
}
