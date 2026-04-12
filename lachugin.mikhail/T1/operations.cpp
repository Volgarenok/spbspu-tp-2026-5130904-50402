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
  auto new_note = std::make_shared<Note>(note);
  db[note] = new_note;
}

void lachugin::addLine(std::istream &in, std::ostream &, notesMap &db)
{
  std::string note;
  in >> note;
  if (db[note])
  {
    std::string str;
    in >> std::quoted(str);
    db[note]->lines.push_back(str);
  }
}

