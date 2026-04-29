#include "op.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>

void novikov::note(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string name;
  in >> name;
  auto note = db.find(name);
  if (note != db.end()) {
    throw std::logic_error("Note already exists");
  }
  auto new_note = std::make_shared< Note >(name);
  db[name] = new_note;
}

void novikov::line(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string name;
  in >> name;
  std::string text;
  in >> std::quoted(text);
  auto note = db.find(name);
  if (note == db.end()) {
    throw std::logic_error("Note not exist");
  }
  note->second->lines.push_back(text);
}

void novikov::show(std::istream &in, std::ostream &out, mapNotes &db)
{
  std::string name;
  in >> name;
  auto note = db.find(name);
  if (note == db.end()) {
    throw std::logic_error("Note not exist");
  }

  if (note->second->lines.empty()) {
    out << "\n";
    return;
  }

  for (size_t i = 0; i < note->second->lines.size(); ++i) {
    out << note->second->lines[i] << "\n";
  }
}

void novikov::drop(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string name;
  in >> name;
  if (db.find(name) == db.end()) {
    throw std::logic_error("Note not exist");
  }
  db.erase(name);
}

void novikov::link(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string note_from;
  std::string note_to;
  in >> note_from >> note_to;
  auto note1 = db.find(note_from);
  auto note2 = db.find(note_to);
  if (note1 == db.end()) {
    throw std::logic_error("<note-from> not exist");
  }
  if (note2 == db.end()) {
    throw std::logic_error("<note-to> not exist");
  }

  for (size_t i = 0; i < note1->second->links.size(); ++i) {
    if (note1->second->links[i].lock() == note2->second) {
      throw std::logic_error("Dublicate");
    }
  }
  note1->second->links.push_back(note2->second);
}

void novikov::halt(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string note_from;
  std::string note_to;
  in >> note_from >> note_to;
  auto note1 = db.find(note_from);
  auto note2 = db.find(note_to);
  if (note1 == db.end()) {
    throw std::logic_error("<note-from> not exist");
  }
  if (note2 == db.end()) {
    throw std::logic_error("<note-to> not exist");
  }
  for (size_t i = 0; i < note1->second->links.size(); ++i) {
    if (note1->second->links[i].lock() == note2->second) {
      note1->second->links.erase(note1->second->links.begin() + i);
      return;
    }
  }
}

void novikov::mind(std::istream &in, std::ostream &out, mapNotes &db)
{
  std::string name;
  in >> name;
  auto note = db.find(name);
  if (note == db.end()) {
    throw std::logic_error("Note not exist");
  }

  if (note->second->links.empty()) {
    out << "\n";
    return;
  }

  for (size_t i = 0; i < note->second->links.size(); ++i) {
    auto link = note->second->links[i].lock();
    if (link) {
      out << link->name << "\n";
    }
  }
}

void novikov::expired(std::istream &in, std::ostream &out, mapNotes &db)
{
  std::string name;
  in >> name;
  auto note = db.find(name);
  if (note == db.end()) {
    throw std::logic_error("Note not exist");
  }
  size_t res = 0;
  for (size_t i = 0; i < note->second->links.size(); ++i) {
    auto link = note->second->links[i].lock();
    if (!link) {
      ++res;
    }
  }
  out << res << "\n";
}

void novikov::refresh(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string name;
  in >> name;
  auto note = db.find(name);
  if (note == db.end()) {
    throw std::logic_error("Note not exist");
  }
  size_t i = 0;
  while (i < note->second->links.size()) {
    auto link = note->second->links[i].lock();
    if (!link) {
      note->second->links.erase(note->second->links.begin() + i);
    } else {
      ++i;
    }
  }
}
