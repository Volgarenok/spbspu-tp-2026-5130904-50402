#include <stdexcept>
#include <memmory>
#include "action.hpp"

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
