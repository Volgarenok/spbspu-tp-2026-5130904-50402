#ifndef CMD_HPP
#define CMD_HPP

#include "note.hpp"

namespace afanasev
{
  using note_t = std::unordered_map< std::string, std::shared_ptr< Note > >;
  using func_t = void (*)(std::istream &, std::ostream &, note_t &);

  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();

  void noteCmd(std::istream & in, std::ostream &, note_t & db);
  void lineCmd(std::istream & in, std::ostream &, note_t & db);
  void showCmd(std::istream & in, std::ostream & out, note_t & db);
  void dropCmd(std::istream & in, std::ostream &, note_t & db);

  void linkCmd(std::istream & in, std::ostream &, note_t & db);
  void haltCmd(std::istream & in, std::ostream &, note_t & db);
  void mindCmd(std::istream & in, std::ostream & out, note_t & db);
  void expiredCmd(std::istream & in, std::ostream &, note_t & db);
  void refreshCmd(std::istream & in, std::ostream &, note_t & db);
}

#endif
