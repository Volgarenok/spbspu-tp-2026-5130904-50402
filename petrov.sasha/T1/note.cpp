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
    db[name] = std::make_shared< Note >(name);
  } else {
    throw std::logic_error("This note already exists");
  }
}

void petrov::addDesc(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name, desc;
  is >> name;
  is >> std::quoted(desc);
  try {
    db.at(name)->desc.push_back(desc);
  } catch (const std::out_of_range &) {
    throw std::logic_error("no note with this name");
  }
}

void petrov::printNote(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  is >> name;
  try {
    if (!db.at(name)->desc.size()) {
      os << '\n';
    }
    for (const std::string &line : db.at(name)->desc) {
      os << line << '\n';
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("no note with this name");
  }
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
  try {
    if (petrov::find(db.at(from)->links.begin(), db.at(from)->links.end(), to) == db.at(from)->links.end()) {
      db.at(from)->links.push_back(db.at(to));
    } else {
      throw std::logic_error("This link already exists");
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("Cant link");
  }
}

