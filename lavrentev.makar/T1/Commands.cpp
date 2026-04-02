#include "Commands.hpp"
#include <memory>
#include <stdexcept>
#include <iomanip>
#include <unordered_map>

void lavrentev::note(std::istream &in,
    std::ostream &,
    std::unordered_map<std::string,
    std::shared_ptr<Note>> &db)
{
  std::string newName;
  in >> newName;
  if (db.count(newName) != 0)
  {
    throw std::logic_error("Node is already exists");
  }
  std::shared_ptr<Note> newNote = std::make_shared<Note>();
  newNote->name = newName;
  db[newName] = newNote;
}

void lavrentev::line(std::istream &in,
    std::ostream &,
    std::unordered_map<std::string,
    std::shared_ptr<Note>> &db)
{
  std::string name, text;
  in >> name;
  if (db.count(name) == 0)
  {
    throw std::logic_error("No such Note");
  }
  in >> std::quoted(text);
  db[name]->lines.push_back(text);
}

void lavrentev::show(std::istream &in,
    std::ostream &out,
    std::unordered_map<std::string,
    std::shared_ptr<Note>> &db)
{
  std::string name;
  in >> name;
  if (db.count(name) != 0)
  {
    for (size_t i = 0; i < db[name]->lines.size(); ++i)
    {
      out << db[name]->lines[i] << "\n";
    }
    return;
  }
  throw std::logic_error("No such Note");
}

void lavrentev::drop(std::istream &in,
    std::ostream &,
    std::unordered_map<std::string,
    std::shared_ptr<Note>> &db)
{
  std::string name;
  in >> name;
  if (db.count(name) != 0)
  {
    db.erase(name);
    return;
  }
  throw std::logic_error("No such Note");
}

void lavrentev::link(std::istream &in,
    std::ostream &,
    std::unordered_map<std::string,
    std::shared_ptr<Note>> &db)
{
  std::string noteTo, noteFrom;
  in >> noteFrom >> noteTo;
  if (db.count(noteTo) == 0 || db.count(noteFrom) == 0)
  {
    throw std::logic_error("No such Note");
  }
  for (size_t i = 0; i < db[noteFrom]->ptrs.size(); ++i)
  {
    std::shared_ptr<Note> k = db[noteFrom]->ptrs[i].lock();
    if (k != nullptr)
    {
      if (k->name == noteTo)
      {
        throw std::logic_error("Link fot this Note is already exists");
      }
    }
  }
  std::weak_ptr<Note> newLink = db[noteTo];
  db[noteFrom]->ptrs.push_back(newLink);
}

void lavrentev::mind(std::istream &in,
    std::ostream &out,
    std::unordered_map<std::string,
    std::shared_ptr<Note>> &db)
{
  std::string name;
  in >> name;
  if (db.count(name) != 0)
  {
    for (size_t i = 0; i < db[name]->ptrs.size(); ++i)
    {
      std::shared_ptr<Note> k = db[name]->ptrs[i].lock();
      out << k->name << "\n";
    }
    return;
  }
  throw std::logic_error("No such Note");
}

void lavrentev::halt(std::istream &in,
    std::ostream &,
    std::unordered_map<std::string,
    std::shared_ptr<Note>> &db)
{
  std::string noteTo, noteFrom;
  in >> noteFrom >> noteTo;
  if (db.count(noteTo) == 0 || db.count(noteFrom) == 0)
  {
    throw std::logic_error("No such Note");
  }
  auto &ptrs = db.find(noteFrom)->second->ptrs;
  for (size_t i = 0; i < ptrs.size(); ++i)
  {
    std::shared_ptr<Note> k = ptrs[i].lock();
    if (k != nullptr)
    {
      if (k->name == noteTo)
      {
        ptrs.erase(ptrs.begin() + i);
        return;
      }
    }
  }
  throw std::logic_error("Link not found");
}

void lavrentev::expired(
    std::istream &in, std::ostream &out,
    std::unordered_map<std::string, std::shared_ptr<Note>> &db)
{
  std::string name;
  in >> name;
  if (db.count(name) == 0)
  {
    throw std::logic_error("No such Note");
  }
  size_t count = 0;
  std::shared_ptr<Note> notePtr = db[name];
  for (auto wp : notePtr->ptrs)
  {
    if (wp.expired())
    {
      ++count;
    }
  }
  out << count << "\n";
}

void lavrentev::refresh(
    std::istream &in, std::ostream &,
    std::unordered_map<std::string, std::shared_ptr<Note>> &db)
{
  std::string name;
  in >> name;
  if (db.count(name) == 0)
  {
    throw std::logic_error("No such Note");
  }
  std::shared_ptr<Note> notePtr = db[name];
  for (auto it = notePtr->ptrs.begin(); it != notePtr->ptrs.end();)
  {
    if (it->expired())
    {
      it = notePtr->ptrs.erase(it);
    }
    else
    {
      ++it;
    }
  }
}
