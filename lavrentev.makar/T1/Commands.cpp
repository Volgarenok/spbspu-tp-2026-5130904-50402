#include "Commands.hpp"
#include <stdexcept>
#include <iomanip>

void lavrentev::note(std::istream& in, std::ostream&, std::vector< std::shared_ptr<Note> >& db)
{
  std::string newName;
  in >> newName;
  for(size_t i = 0; i < db.size(); ++i)
  {
    if(db[i]->name == newName)
    {
      throw std::logic_error("Node is already exists");
    }
  }
  std::shared_ptr<Note> newNote{};
  newNote->name = newName;
  db.push_back(newNote);
}
