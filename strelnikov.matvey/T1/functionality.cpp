#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "functionality.hpp"

struct strelnikov::Note
{
  Note(std::string);
  std::string name;
  std::vector< std::string > lines;
  std::vector< std::weak_ptr< Note > > links;
};

strelnikov::Note::Note(std::string str):
  name(str),
  lines(),
  links()
{}

strelnikov::It_t strelnikov::findLink(It_t start, It_t end, std::string name)
{
  while (start != end) {
    if (!start->expired() && start->lock()->name == name) {
      return start;
    }
    ++start;
  }
  return end;
}

void strelnikov::addNote(std::istream &in, std::ostream &, strelnikov::db_t &db)
{
  std::string str;
  in >> str;

  if (db.find(str) == db.end()) {
    db[str] = std::make_shared< Note >(str);
  } else {
    throw std::logic_error("Note already exist");
  }
}

void strelnikov::addLine(std::istream &in, std::ostream &, strelnikov::db_t &db)
{
  std::string str, line;
  in >> str;
  in >> std::quoted(line);

  try {
    db.at(str)->lines.push_back(line);
  } catch (const std::out_of_range &) {
    throw std::logic_error("Note does not exist");
  }
}

void strelnikov::show(std::istream &in, std::ostream &out, strelnikov::db_t &db)
{
  std::string str;
  in >> str;

  try {
    for (std::string s : db.at(str)->lines) {
      out << s << '\n';
    }
    if (db.at(str)->lines.empty()) {
      out << '\n';
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("Note does not exist");
  }
}

void strelnikov::drop(std::istream &in, std::ostream &, strelnikov::db_t &db)
{
  std::string str;
  in >> str;
  if (db.find(str) != db.end()) {
    db.erase(str);
  } else {
    throw std::logic_error("Note does not exist");
  }
}

void strelnikov::link(std::istream &in, std::ostream &, strelnikov::db_t &db)
{
  std::string from, to;
  in >> from >> to;

  try {
    std::vector< std::weak_ptr< Note > > &links_tmp = db.at(from)->links;
    if (findLink(links_tmp.begin(), links_tmp.end(), to) == links_tmp.end()) {
      db.at(from)->links.push_back(db.at(to));
    } else {
      throw std::logic_error("Notes already linked");
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("Notes cant be linked");
  }
}

void strelnikov::showMindMap(std::istream &in, std::ostream &out, strelnikov::db_t &db)
{
  std::string from;
  in >> from;

  bool flag = true;
  try {
    std::vector< std::weak_ptr< Note > > &links_tmp = db.at(from)->links;
    for (const std::weak_ptr< Note > &l : links_tmp) {
      if (!l.expired()) {
        out << l.lock()->name << '\n';
        flag = false;
      }
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("Note does not exist");
  }

  if (flag) {
    out << '\n';
  }
}

void strelnikov::halt(std::istream &in, std::ostream &, strelnikov::db_t &db)
{
  std::string from, to;
  in >> from >> to;

  try {
    std::vector< std::weak_ptr< Note > > &links_tmp = db.at(from)->links;
    strelnikov::It_t it = strelnikov::findLink(links_tmp.begin(), links_tmp.end(), to);
    if (it == links_tmp.end()) {
      throw std::logic_error("No link");
    }
    links_tmp.erase(it);
  } catch (const std::out_of_range &) {
    throw std::logic_error("Note does not exist");
  }
}

void strelnikov::expired(std::istream &in, std::ostream &out, strelnikov::db_t &db)
{
  std::string str;
  size_t s = 0;
  in >> str;
  try {
    for (const std::weak_ptr< Note > &link : db.at(str)->links) {
      if (link.expired()) {
        ++s;
      }
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("No note with this name");
  }
  out << s << '\n';
}

void strelnikov::refresh(std::istream &in, std::ostream &, strelnikov::db_t &db)
{
  std::string str;
  std::vector< std::weak_ptr< Note > > v;
  in >> str;
  try {
    for (std::weak_ptr< Note > &link : db.at(str)->links) {
      if (!link.expired()) {
        v.push_back(std::move(link));
      }
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("No note with this name");
  }
  db.at(str)->links.swap(v);
}
