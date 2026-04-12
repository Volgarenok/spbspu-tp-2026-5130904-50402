#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "note.hpp"

namespace lachugin
{
  using notesMap = std::unordered_map< std::string, std::shared_ptr< Note > >;
  using cmd = void (*)(std::istream&, std::ostream&, notesMap&);

  void addNote(std::istream&, std::ostream&, notesMap&);
  void addLine(std::istream&, std::ostream&, notesMap&);
  void showNote(std::istream&, std::ostream&, notesMap&);
  void dropNote(std::istream&, std::ostream&, notesMap&);
  void linkTo(std::istream&, std::ostream&, notesMap&);
  void printLinks(std::istream&, std::ostream&, notesMap&);
  void removeLink(std::istream&, std::ostream&, notesMap&);
  void allRemovedNotes(std::istream&, std::ostream&, notesMap&);
  void linksRemover(std::istream&, std::ostream&, notesMap&);
}

#endif
