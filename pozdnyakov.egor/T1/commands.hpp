#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "note.hpp"

namespace pozdnyakov
{

  using Database = std::unordered_map< std::string, std::shared_ptr< Note > >;

  void cmdNote(std::istream &in, std::ostream &out, Database &db);
  void cmdLine(std::istream &in, std::ostream &out, Database &db);
  void cmdShow(std::istream &in, std::ostream &out, Database &db);
  void cmdDrop(std::istream &in, std::ostream &out, Database &db);
  void cmdLink(std::istream &in, std::ostream &out, Database &db);
  void cmdMind(std::istream &in, std::ostream &out, Database &db);
  void cmdHalt(std::istream &in, std::ostream &out, Database &db);
  void cmdExpired(std::istream &in, std::ostream &out, Database &db);
  void cmdRefresh(std::istream &in, std::ostream &out, Database &db);

}

#endif
