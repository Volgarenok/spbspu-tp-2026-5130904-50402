#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <unordered_map>
#include "note.hpp"
namespace sogdanov
{
  using NotePtr = std::shared_ptr<Note>;
  using NoteMap = std::unordered_map<std::string, NotePtr>;
  void cmd_note(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmd_line(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmd_show(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmd_drop(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmd_link(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmd_halt(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmd_mind(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmd_expired(std::istream &in, std::ostream &out, NoteMap &notes);
  void cmd_refresh(std::istream &in, std::ostream &out, NoteMap &notes);
}
#endif

