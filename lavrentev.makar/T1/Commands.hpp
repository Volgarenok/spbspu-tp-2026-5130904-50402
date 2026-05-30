#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include "Note.hpp"

namespace lavrentev
{
  void note(std::istream &in, std::ostream &, std::unordered_map<std::string, std::shared_ptr<Note>> &db);
  void line(std::istream &in, std::ostream &, std::unordered_map<std::string, std::shared_ptr<Note>> &db);
  void show(std::istream &in, std::ostream &out, std::unordered_map<std::string, std::shared_ptr<Note>> &db);
  void drop(std::istream &in, std::ostream &, std::unordered_map<std::string, std::shared_ptr<Note>> &db);
  void link(std::istream &in, std::ostream &, std::unordered_map<std::string, std::shared_ptr<Note>> &db);
  void halt(std::istream &in, std::ostream &, std::unordered_map<std::string, std::shared_ptr<Note>> &db);
  void mind(std::istream &in, std::ostream &out, std::unordered_map<std::string, std::shared_ptr<Note>> &db);
  void expired(std::istream &in, std::ostream &out, std::unordered_map<std::string, std::shared_ptr<Note>> &db);
  void refresh(std::istream &in, std::ostream &, std::unordered_map<std::string, std::shared_ptr<Note>> &db);

  using cmd_t = void (*)(std::istream &, std::ostream &,
    std::unordered_map<std::string, std::shared_ptr<lavrentev::Note>> &);
}

#endif
