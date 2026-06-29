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
  using cmd_t = void (*)(std::istream&, std::ostream&, notesMap&);

  void addNote(std::istream&, std::ostream&, notesMap&);
  void addLine(std::istream&, std::ostream&, notesMap&);
  void showNote(std::istream&, std::ostream&, notesMap&);
  void removeNote(std::istream&, std::ostream&, notesMap&);
  void addLink(std::istream&, std::ostream&, notesMap&);
  void printLinks(std::istream&, std::ostream&, notesMap&);
  void removeLink(std::istream&, std::ostream&, notesMap&);
  void allRemovedNotes(std::istream&, std::ostream&, notesMap&);
  void linksRemover(std::istream&, std::ostream&, notesMap&);

  bool dfsLoop(const std::shared_ptr< Note >& start, const std::shared_ptr< Note >& cur,
    size_t d, size_t max, std::vector< std::shared_ptr< Note > >& path);
  void loop(std::istream&, std::ostream&, notesMap&);
}

#endif
