#ifndef OP_HPP
#define OP_HPP
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "note.hpp"

namespace novikov
{
  using mapNotes = std::unordered_map< std::string, std::shared_ptr< Note > >;
  using cmd_t = void (*)(std::istream &, std::ostream &, mapNotes &);

  void note(std::istream &, std::ostream &, mapNotes &);
  void line(std::istream &, std::ostream &, mapNotes &);
  void show(std::istream &, std::ostream &, mapNotes &);
  void drop(std::istream &, std::ostream &, mapNotes &);
  void link(std::istream &, std::ostream &, mapNotes &);
  void halt(std::istream &, std::ostream &, mapNotes &);
  void mind(std::istream &, std::ostream &, mapNotes &);
  void expired(std::istream &, std::ostream &, mapNotes &);
  void refresh(std::istream &, std::ostream &, mapNotes &);
}

#endif