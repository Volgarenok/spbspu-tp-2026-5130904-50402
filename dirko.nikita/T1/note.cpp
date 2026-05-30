#include "note.hpp"
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

dirko::Note::Note(std::string name):
  name(name),
  desc(),
  links()
{}

dirko::linkIt_t dirko::find(linkIt_t start, linkIt_t end, std::string name)
{
  for (; start != end; ++start) {
    if (!start->expired() && start->lock()->name == name) {
      return start;
    }
  }
  return end;
}

void dirko::addNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name;
  is >> name;
  if (db.find(name) == db.end()) {
    db.emplace(name, std::make_shared< Note >(name));
  } else {
    throw std::logic_error("This note already exists");
  }
}

void dirko::addDesc(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name, desc;
  is >> name;
  is >> std::quoted(desc);
  db.at(name)->desc.push_back(desc);
}

void dirko::printNote(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  is >> name;
  std::vector< std::string >::const_iterator begin = db.at(name)->desc.cbegin();
  std::vector< std::string >::const_iterator end = db.at(name)->desc.cend();
  if (begin != end) {
    os << *begin;
    for (std::vector< std::string >::const_iterator it = ++begin; it != end; ++it) {
      os << '\n' << (*it);
    }
  }
}

void dirko::dropNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name;
  is >> name;
  if (db.find(name) == db.end()) {
    throw std::logic_error("no note with this name");
  }
  db.erase(name);
}

void dirko::linkNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string from, to;
  is >> from >> to;
  if (dirko::find(db.at(from)->links.begin(), db.at(from)->links.end(), to) == db.at(from)->links.end()) {
    db.at(from)->links.push_back(db.at(to));
  } else {
    throw std::logic_error("This link already exists");
  }
}
void dirko::removeLink(std::istream &is, std::ostream &, notes_t &db)
{
  std::string from, to;
  is >> from >> to;
  dirko::linkIt_t it = dirko::find(db.at(from)->links.begin(), db.at(from)->links.end(), to);
  if (it == db.at(from)->links.end()) {
    throw std::logic_error("No link with this name");
  }
  db.at(from)->links.erase(it);
}
void dirko::printLinks(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  is >> name;
  bool first = true;
  std::vector< std::weak_ptr< Note > > &links = db.at(name)->links;
  for (const std::weak_ptr< Note > &link : links) {
    if (!link.expired()) {
      os << (first ? "" : "\n") << link.lock()->name;
      first = false;
    }
  }
}

void dirko::countExpired(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  size_t expired = 0;
  is >> name;
  std::vector< std::weak_ptr< Note > > &links = db.at(name)->links;
  for (const std::weak_ptr< Note > &link : links) {
    if (link.expired()) {
      ++expired;
    }
  }
  os << expired;
}

void dirko::refreshLinks(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name;
  std::vector< std::weak_ptr< Note > > vec;
  is >> name;
  std::vector< std::weak_ptr< Note > > &links = db.at(name)->links;
  for (std::weak_ptr< Note > &link : links) {
    if (!link.expired()) {
      vec.push_back(std::move(link));
    }
  }
  db.at(name)->links.swap(vec);
}
