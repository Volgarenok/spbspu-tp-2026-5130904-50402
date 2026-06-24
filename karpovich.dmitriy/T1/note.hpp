#ifndef NOTE_HPP
#define NOTE_HPP

#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace karpovich
{
  class Note
  {
  public:
    std::string name;
    std::vector< std::string > lines;
    std::vector< std::pair< std::string, std::weak_ptr< Note > > > links;
    Note(const std::string &name);
  };

  using NoteMap = std::unordered_map< std::string, std::shared_ptr< Note > >;

  void cmdNote(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmdLine(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmdShow(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmdDrop(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmdLink(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmdHalt(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmdMind(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmdExpired(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmdRefresh(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmdLoop(std::istream &in, std::ostream &out, NoteMap &notes);
}

#endif
