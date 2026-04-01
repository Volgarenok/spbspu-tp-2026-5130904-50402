#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <iomanip>
#include "Note.hpp"

using cmd_t = void(*)(std::istream&, std::ostream&, std::vector< lavrentev::Note >&);

namespace lavrentev{
  void note(std::istream& in, std::ostream&, std::vector< lavrentev::Note >& db);
  void line(std::istream& in, std::ostream&, std::vector< lavrentev::Note >& db);
  void show(std::istream& in, std::ostream& out, std::vector< lavrentev::Note >& db);
  void drop(std::istream& in, std::ostream&, std::vector< lavrentev::Note >& db);

  void link(std::istream& in, std::ostream&, std::vector< lavrentev::Note >& db);
  void halt(std::istream& in, std::ostream&, std::vector< lavrentev::Note >& db);
  void mind(std::istream& in, std::ostream& out, std::vector< lavrentev::Note >& db);
  void expired(std::istream& in, std::ostream& out, std::vector< lavrentev::Note >& db);
  void refresh(std::istream& in, std::ostream&, std::vector< lavrentev::Note >& db);
}

#endif