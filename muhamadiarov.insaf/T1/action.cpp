#include <stdexcept>
#include <memory>
#include <iomanip>
#include "action.hpp"

namespace muh = muhamadiarov;

void muh::note(std::istream& in, std::ostream&, NoteMap_t& map)
{
  std::string str;
  if (!(in >> str))
  {
    throw std::logic_error("Not create name");
  }
  if (str.empty())
  {
    throw std::logic_error("the name must consist of letters");
  }
  if (map.find(str) != map.end())
  {
    throw std::logic_error("Note already exist");
  }
  std::shared_ptr< Note > note = std::make_shared< Note >();
  note->name_ = str;
  map[str] = note;
}

void muh::line(std::istream& in, std::ostream&, NoteMap_t& map)
{
  std::string str;
  std::string ln;
  if (!(in >> str >> std::quoted(ln)))
  {
    throw std::logic_error("Incorrect command");
  }
  if (map.find(str) != map.end())
  {
    map.at(str)->lines_.push_back(ln);
  }
  else
  {
    throw std::logic_error("Not find the cuurent note");
  }
}

void muh::show(std::istream& in, std::ostream& out, NoteMap_t& map)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::logic_error("Not name-note");
  }
  NoteMap_t::const_iterator iter = map.find(name);
  if (iter == map.end())
  {
    throw std::logic_error("Not find this note");
  }
  if (iter->second->lines_.size() == 0)
  {
    out << '\n';
    return;
  }
  for (size_t i = 0; i < iter->second->lines_.size(); ++i)
  {
    out << iter->second->lines_[i] << '\n';
  }
}

void muh::drop(std::istream& in, std::ostream&, NoteMap_t& map)
{
  std::string str;
  if (!(in >> str))
  {
    throw std::logic_error("Not name-note");
  }
  NoteMap_t::iterator iter = map.find(str);
  if (iter == map.end())
  {
    throw std::logic_error("Not find this note");
  }
  map.erase(iter);
}

void muh::link(std::istream& in, std::ostream&, NoteMap_t& map)
{
  std::string from, to;
  if (!(in >> from >> to))
  {
    throw std::logic_error("Not correct arguments");
  }
  NoteMap_t::iterator itFrom = map.find(from);
  NoteMap_t::iterator itTo = map.find(to);
  if (itFrom == map.end() || itTo == map.end())
  {
    throw std::logic_error("Not find these notes");
  }
  for (size_t i = 0; i < itFrom->second->links_.size(); ++i)
  {
    if (itFrom->second->links_[i].first == to)
    {
      throw std::logic_error("Link already exists");
    }
  }
  itFrom->second->links_.emplace_back(to, std::weak_ptr< Note >(itTo->second));
}

void muh::halt(std::istream& in, std::ostream&, NoteMap_t& map)
{
  std::string from, to;
  if (!(in >> from >> to))
  {
    throw std::logic_error("Not correct arguments");
  }
  NoteMap_t::iterator itFrom = map.find(from);
  NoteMap_t::iterator itTo = map.find(to);
  if (itFrom == map.end() || itTo == map.end())
  {
    throw std::logic_error("Not find these notes");
  }
  bool hasFoundNote = false;
  auto it = itFrom->second->links_.begin();
  for (size_t i = 0; i < itFrom->second->links_.size(); ++i)
  {
    if ((*it).first == to)
    {
      hasFoundNote = true;
      itFrom->second->links_.erase(it);
    }
    ++it;
  }
  if (!hasFoundNote)
  {
    throw std::logic_error("Not find to-note");
  }
}

void muh::mind(std::istream& in, std::ostream& out, NoteMap_t& map)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::logic_error("Not name-note");
  }
  NoteMap_t::const_iterator iter = map.find(name);
  if (iter == map.end())
  {
    throw std::logic_error("Not find this note");
  }
  if (iter->second->links_.size() == 0)
  {
    out << '\n';
    return;
  }
  for (size_t i = 0; i < iter->second->links_.size(); ++i)
  {
    if (!(iter->second->links_[i].second.expired()))
    {
      out << iter->second->links_[i].first << '\n';
    }
  }
}

void muh::expired(std::istream& in, std::ostream& out, NoteMap_t& map)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::logic_error("Not name-note");
  }
  NoteMap_t::const_iterator iter = map.find(name);
  if (iter == map.end())
  {
    throw std::logic_error("Not find this note");
  }
  if (iter->second->links_.size() == 0)
  {
    out << '\n';
    return;
  }
  size_t count = 0;
  for (size_t i = 0; i < iter->second->links_.size(); ++i)
  {
    if (iter->second->links_[i].second.expired())
    {
      ++count;
    }
  }
  out << count << '\n';
}

void muh::refresh(std::istream& in, std::ostream&, NoteMap_t& map)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::logic_error("Not name-note");
  }
  NoteMap_t::iterator iter = map.find(name);
  if (iter == map.end())
  {
    throw std::logic_error("Not find this note");
  }
  std::vector< std::pair< std::string, std::weak_ptr< Note > > > links = iter->second->links_;
  size_t countCorrect = 0;
  for (size_t i = 0; i < links.size(); ++i)
  {
    if (!(links[i].second.expired()))
    {
      if (countCorrect != i)
      {
        links[countCorrect] = links[i];
      }
      ++countCorrect;
    }
  }
  links.resize(countCorrect);
}
