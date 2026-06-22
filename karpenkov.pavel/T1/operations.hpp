#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP
#include <iostream>
#include "note.hpp"
#include <unordered_map>
namespace karpenkov
{
  using mapOfNotes = std::unordered_map< std::string, std::shared_ptr< Note > >;
  void addNote(std::istream &in, std::ostream &, mapOfNotes &notes);
  void addLine(std::istream &in, std::ostream &, mapOfNotes &notes);
  void showNote(std::istream &in, std::ostream &out, mapOfNotes &notes);
  void dropNote(std::istream &in, std::ostream &, mapOfNotes &notes);
  void addLink(std::istream &in, std::ostream &, mapOfNotes &notes);
  void removeLink(std::istream &in, std::ostream &, mapOfNotes &notes);
  void showLinks(std::istream &in, std::ostream &out, mapOfNotes &notes);
  void showExpired(std::istream &in, std::ostream &out, mapOfNotes &notes);
  void refreshLinks(std::istream &in, std::ostream &, mapOfNotes &notes);
}
#endif
