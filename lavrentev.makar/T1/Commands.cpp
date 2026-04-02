#include "Commands.hpp"
#include <stdexcept>
#include <iomanip>
#include <unordered_map>
#include <map>

std::map<std::string, std::shared_ptr<lavrentev::Note> > deletedNotes;

void lavrentev::note(std::istream& in, std::ostream&, std::unordered_map<std::string, std::shared_ptr<Note> >& db)
{
  std::string newName;
  in >> newName;
  if(db.count(newName) != 0)
  {
    throw std::logic_error("Node is already exists");
  }
  std::shared_ptr<Note> newNote{};
  db[newName] = newNote;
}

void lavrentev::line(std::istream& in, std::ostream&, std::unordered_map<std::string, std::shared_ptr<Note> >& db)
{
  std::string name, text;
  in >> name;
  in >> std::quoted(text);
  if(db.count(name) != 0)
  {
    db[name]->lines.push_back(text);
    return;
  }
  throw std::logic_error("No such Note");
}

void lavrentev::show(std::istream& in, std::ostream& out, std::unordered_map<std::string, std::shared_ptr<Note> >& db)
{
  std::string name;
  in >> name;
  if(db.count(name) != 0)
  {
    for(size_t i = 0; i < db[name]->lines.size(); ++i)
    {
      out << db[name]->lines[i] << "\n";
    }
    return;
  }
  throw std::logic_error("No such Note");
}

void lavrentev::drop(std::istream& in, std::ostream&, std::unordered_map<std::string, std::shared_ptr<Note> >& db)
{
  std::string name;
  in >> name;
  if(db.count(name) != 0)
  {
    std::shared_ptr<Note> toDelete = db[name];
    db.erase(name);
    deletedNotes[name] = toDelete;
    return;
  }
  throw std::logic_error("No such Note");
}
