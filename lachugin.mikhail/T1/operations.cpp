#include "operations.hpp"
#include "note.hpp"
#include <vector>
#include <iomanip>
#include <string>
#include <unordered_map>

void lachugin::addNote(std::istream& in, std::ostream &, notesMap &db)
{
  std::string note;
  in >> note;
  auto it = db.find(note);
  if (it != db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }
  auto new_note = std::make_shared<Note>(note);
  db[note] = new_note;
}

void lachugin::addLine(std::istream &in, std::ostream &, notesMap &db)
{
  std::string note;
  in >> note;

  auto it = db.find(note);
  if (it == db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }

  std::string str;
  in >> std::quoted(str);
  db[note]->lines.push_back(str);

}

void lachugin::showNote(std::istream &in, std::ostream &out, notesMap &db)
{
  std::string note;
  in >> note;

  auto it = db.find(note);
  if (it == db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }
  for (size_t i = 0; i < it->second->lines.size(); ++i)
  {
    out << it->second->lines[i] << "\n";
  }
}


