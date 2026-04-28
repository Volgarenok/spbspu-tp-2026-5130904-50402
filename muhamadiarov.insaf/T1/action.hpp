#ifndef ACTION_HPP
#define ACTION_HPP
#include <istream>
#include <ostream>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

namespace muhamadiarov
{
  struct Note
  {
    std::string name_;
    std::vector< std::string > lines_;
    std::vector< std::pair< std::string, std::weak_ptr< Note > > > links_;
  };
  
  using NoteMap_t = std::unordered_map<std::string, std::shared_ptr< Note > >;
  
  void note(std::istream& in, std::ostream& out, NoteMap_t& map);
  void line(std::istream& in, std::ostream& out, NoteMap_t& map);
  void show(std::istream& in, std::ostream& out, NoteMap_t& map);
  void drop(std::istream& in, std::ostream& out, NoteMap_t& map);
  void link(std::istream& in, std::ostream& out, NoteMap_t& map);
  void halt(std::istream& in, std::ostream& out, NoteMap_t& map);
  void mind(std::istream& in, std::ostream& out, NoteMap_t& map);
  void expired(std::istream& in, std::ostream& out, NoteMap_t& map);
  void refresh(std::istream& in, std::ostream& out, NoteMap_t& map);
}
#endif
