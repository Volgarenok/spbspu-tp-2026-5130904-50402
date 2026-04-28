#include <stdexcept>
#include <memmory>
#include "action.hpp"
#include <iomanip>

namespace muh = muhamadiarov;

void muh::note(std::istream& in, std::ostream& out, NoteMap_t& map)
{
  std::string str;
  if (!(in >> str))
  {
    throw std::logic_error("Not create name");
  }
  if (str.empty())
  {
    throw std::logic_error("the name must consist of letters");
  }
  if (map.find(str) != map.end())
  {
    throw std::logic_error("Note already exist");
  }
  std::shared_ptr< Note > note = std::make_shared< Note >();
  note->name_ = str;
  map[str] = note;
}

void muh::line(std::istream& in, std::ostream& out, NoteMap_t& map)
{
  std::string str;
  std::string ln;
  if (!(in >> str >> std::quoted(ln)))
  {
    throw std::logic_error("Incorrect command");
  }
  if (map.find(str) != map.end())
  {
    map.at(str)->lines_.push_back(ln);
  }
  else
  {
    throw std::logic_error("Not find the cuurent note");
  }
}

void muh::show(std::istream& in, std::ostream& out, NoteMap_t& map)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::logic_error("Not name-note");
  }
  NoteMap_t::const_iterator iter = map.find(name);
  if (iter == map.end())
  {
    throw std::logic_error("Not find this note");
  }
  if (iter->second->lines_.size() == 0)
  {
    out << '\n';
  }
  for (size_t i = 0; i < iter->second->lines_.size(); ++i)
  {
    out << iter->second->lines_[i] << '\n';
  }
}
