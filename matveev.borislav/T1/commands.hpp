#ifndef MATVEEV_COMMANDS_HPP
#define MATVEEV_COMMANDS_HPP

#include <iostream>
#include <unordered_map>
#include "note.hpp"

namespace matveev
{
  using cmd_t = void(*)(std::istream&, std::ostream&, db_t&);
  using cmd_map_t = std::unordered_map< std::string, cmd_t >;

  cmd_map_t initCommands();

  void create_note(std::istream&, std::ostream&, db_t&);
  void add_line(std::istream&, std::ostream&, db_t&);
  void show_note(std::istream&, std::ostream&, db_t&);
  void drop_note(std::istream&, std::ostream&, db_t&);
  void link_note(std::istream&, std::ostream&, db_t&);
  void mind_note(std::istream&, std::ostream&, db_t&);
  void halt_note(std::istream&, std::ostream&, db_t&);
  void expired_note(std::istream&, std::ostream&, db_t&);
  void refresh_note(std::istream&, std::ostream&, db_t&);
  void loop_note(std::istream&, std::ostream&, db_t&);
}

#endif
