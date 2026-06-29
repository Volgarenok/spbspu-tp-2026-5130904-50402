#include "op.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>

void novikov::note(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string name;
  in >> name;
  std::shared_ptr< Note > new_note = std::make_shared< Note >(name);
  auto result = db.insert({name, new_note});
  if (!result.second) {
    throw std::logic_error("Note already exists");
  }
}

void novikov::line(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string name;
  in >> name;
  std::string text;
  in >> std::quoted(text);
  std::shared_ptr< Note > &note = db.at(name);
  note->lines.push_back(text);
}

void novikov::show(std::istream &in, std::ostream &out, mapNotes &db)
{
  std::string name;
  in >> name;
  std::shared_ptr< Note > &note = db.at(name);

  const std::vector< std::string > &lines = note->lines;
  if (lines.empty()) {
    return;
  }

  out << lines[0];
  for (size_t i = 1; i < lines.size(); ++i) {
    out << "\n" << lines[i];
  }
}

void novikov::drop(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string name;
  in >> name;
  auto it = db.find(name);
  if (it == db.end()) {
    throw std::logic_error("Note not exist");
  }
  db.erase(it);
}

void novikov::link(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string note_from;
  std::string note_to;
  in >> note_from >> note_to;

  std::shared_ptr< Note > &note1 = db.at(note_from);
  std::shared_ptr< Note > &note2 = db.at(note_to);

  for (size_t i = 0; i < note1->links.size(); ++i) {
    if (note1->links[i].lock() == note2) {
      throw std::logic_error("Dublicate");
    }
  }
  note1->links.push_back(note2);
}

void novikov::halt(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string note_from;
  std::string note_to;
  in >> note_from >> note_to;

  std::shared_ptr< Note > &note1 = db.at(note_from);
  std::shared_ptr< Note > &note2 = db.at(note_to);

  for (size_t i = 0; i < note1->links.size(); ++i) {
    if (note1->links[i].lock() == note2) {
      note1->links.erase(note1->links.begin() + i);
      return;
    }
  }
  throw std::logic_error("Link not exist");
}

void novikov::mind(std::istream &in, std::ostream &out, mapNotes &db)
{
  std::string name;
  in >> name;
  std::shared_ptr< Note > &note = db.at(name);

  bool printed = false;
  for (size_t i = 0; i < note->links.size(); ++i) {
    std::shared_ptr< Note > link = note->links[i].lock();
    if (link) {
      if (printed) {
        out << "\n";
      }
      out << link->name;
      printed = true;
    }
  }
}

void novikov::expired(std::istream &in, std::ostream &out, mapNotes &db)
{
  std::string name;
  in >> name;
  std::shared_ptr< Note > &note = db.at(name);

  size_t res = 0;
  for (size_t i = 0; i < note->links.size(); ++i) {
    std::shared_ptr< Note > link = note->links[i].lock();
    if (!link) {
      ++res;
    }
  }
  out << res;
}

void novikov::refresh(std::istream &in, std::ostream &, mapNotes &db)
{
  std::string name;
  in >> name;
  std::shared_ptr< Note > &note = db.at(name);

  size_t i = 0;
  while (i < note->links.size()) {
    std::shared_ptr< Note > link = note->links[i].lock();
    if (!link) {
      note->links.erase(note->links.begin() + i);
    } else {
      ++i;
    }
  }
}
