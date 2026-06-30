#ifndef NOTE_HPP
#define NOTE_HPP

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace alisov
{
  struct Note
  {
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > links;
  };

  using NotesMap = std::unordered_map< std::string, std::shared_ptr< alisov::Note > >;

  void note(std::istream &in, std::ostream &out, NotesMap &notes);
  void line(std::istream &in, std::ostream &out, NotesMap &notes);
  void show(std::istream &in, std::ostream &out, NotesMap &notes);
  void drop(std::istream &in, std::ostream &out, NotesMap &notes);
  void link(std::istream &in, std::ostream &out, NotesMap &notes);
  void halt(std::istream &in, std::ostream &out, NotesMap &notes);
  void mind(std::istream &in, std::ostream &out, NotesMap &notes);
  void expired(std::istream &in, std::ostream &out, NotesMap &notes);
  void refresh(std::istream &in, std::ostream &out, NotesMap &notes);
}

#endif
