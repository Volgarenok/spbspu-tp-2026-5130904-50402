#ifndef NOTE_HPP
#define NOTE_HPP
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <limits>
#include <vector>

namespace petrov
{
  class Note
  {
  public:
    std::string name;
    std::vector< std::string > desc;
    std::vector< std::weak_ptr< Note > > links;
    Note(std::string name);
  };
  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();
  using notes_t = std::unordered_map< std::string, std::shared_ptr< Note > >;
  using func_t = void (*)(std::istream &, std::ostream &, notes_t &);

  void addNote(std::istream &is, std::ostream &, notes_t &db);
  void addDesc(std::istream &is, std::ostream &, notes_t &db);
  void printNote(std::istream &is, std::ostream &os, notes_t &db);
  void dropNote(std::istream &is, std::ostream &, notes_t &db);
  void linkNote(std::istream &is, std::ostream &, notes_t &db);
  void removeLink(std::istream &is, std::ostream &, notes_t &db);
  void printLinks(std::istream &is, std::ostream &os, notes_t &db);
  void refreshLinks(std::istream &is, std::ostream &, notes_t &db);
  void countExpired(std::istream &is, std::ostream &os, notes_t &db);
  void cmdLoop(std::istream &is, std::ostream &os, notes_t &db);
  bool findLoop(const std::string &startName, const std::string &currentName,
                 size_t depthLeft, notes_t &db, std::vector< std::string > &path);
  using linkIt_t = std::vector< std::weak_ptr< Note > >::iterator;
  linkIt_t find(linkIt_t start, linkIt_t end, const std::string& name);
}

#endif

