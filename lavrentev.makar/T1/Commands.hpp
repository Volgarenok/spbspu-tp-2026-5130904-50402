#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include "Note.hpp"

namespace lavrentev
{
  using dbTemp = std::unordered_map< std::string, std::shared_ptr< Note > >;
  void note(std::istream &in, std::ostream &, dbTemp &db);
  void line(std::istream &in, std::ostream &, dbTemp &db);
  void show(std::istream &in, std::ostream &out, dbTemp &db);
  void drop(std::istream &in, std::ostream &, dbTemp &db);
  void link(std::istream &in, std::ostream &, dbTemp &db);
  void halt(std::istream &in, std::ostream &, dbTemp &db);
  void mind(std::istream &in, std::ostream &out, dbTemp &db);
  void expired(std::istream &in, std::ostream &out, dbTemp &db);
  void refresh(std::istream &in, std::ostream &, dbTemp &db);

  using cmd_t = void (*)(std::istream &, std::ostream &,
    std::unordered_map< std::string, std::shared_ptr< lavrentev::Note > > &);
}

#endif
