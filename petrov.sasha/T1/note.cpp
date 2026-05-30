#include "note.hpp"
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

petrov::Note::Note(std::string name):
  name(name),
  desc(),
  links()
{}

petrov::linkIt_t petrov::find(linkIt_t start, linkIt_t end, std::string name)
{
  for (; start != end; ++start) {
    if (!start->expired() && start->lock()->name == name) {
      return start;
    }
  }
  return end;
}

void petrov::addNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name;
  is >> name;
  if (db.find(name) == db.end()) {
    db.emplace(name, std::make_shared< Note >(name));
  } else {
    throw std::logic_error("This note already exists");
  }
}

void petrov::addDesc(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name, desc;
  is >> name;
  is >> std::quoted(desc);
  db.at(name)->desc.push_back(desc);
}

void petrov::printNote(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  is >> name;
  const auto &desc = db.at(name)->desc;
  if (!desc.empty()) {
    os << desc.front();
    for (auto it = desc.begin() + 1; it != desc.end(); ++it) {
      os << '\n' << *it;
    }
  }
  os << '\n';
}

void petrov::dropNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name;
  is >> name;
  if (db.find(name) == db.end()) {
    throw std::logic_error("no note with this name");
  }
  db.erase(name);
}

void petrov::linkNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string from, to;
  is >> from >> to;
  if (find(db.at(from)->links.begin(), db.at(from)->links.end(), to) == db.at(from)->links.end()) {
    db.at(from)->links.push_back(db.at(to));
  } else {
    throw std::logic_error("This link already exists");
  }
}

void petrov::removeLink(std::istream &is, std::ostream &, notes_t &db)
{
  std::string from, to;
  is >> from >> to;
  linkIt_t it = find(db.at(from)->links.begin(), db.at(from)->links.end(), to);
  if (it == db.at(from)->links.end()) {
    throw std::logic_error("No link with this name");
  }
  db.at(from)->links.erase(it);
}

void petrov::printLinks(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  is >> name;
  const auto &links = db.at(name)->links;
  auto it = links.begin();
  while (it != links.end() && it->expired()) {
    ++it;
  }
  if (it != links.end()) {
    os << it->lock()->name;
    ++it;
    for (; it != links.end(); ++it) {
      if (!it->expired()) {
        os << '\n' << it->lock()->name;
      }
    }
  }
  os << '\n';
}

void petrov::countExpired(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  size_t expired = 0;
  is >> name;
  const auto &links = db.at(name)->links;
  for (const std::weak_ptr< Note > &link : links) {
    if (link.expired()) {
      ++expired;
    }
  }
  os << expired << '\n';
}

void petrov::refreshLinks(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name;
  std::vector< std::weak_ptr< Note > > vec;
  is >> name;
  for (std::weak_ptr< Note > &link : db.at(name)->links) {
    if (!link.expired()) {
      vec.push_back(std::move(link));
    }
  }
  db.at(name)->links.swap(vec);
}

