#include <stdexcept>
#include <memory>
#include <iomanip>
#include "commands.hpp"

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
  map.insert({note->name, note});
}

void muh::line(std::istream& in, std::ostream&, NoteMap_t& map)
{
  std::string str;
  std::string ln;
  if (!(in >> str >> std::quoted(ln)))
  {
    throw std::logic_error("Incorrect command");
  }
  map.at(str)->lines.push_back(ln);
}

void muh::show(std::istream& in, std::ostream& out, NoteMap_t& map)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::logic_error("Not name-note");
  }
  if (map.at(name)->lines.size() != 0)
  {
    out << map.at(name)->lines[0];
  }
  for (size_t i = 1; i < map.at(name)->lines.size(); ++i)
  {
    out << '\n' << map.at(name)->lines[i];
  }
}

void muh::drop(std::istream& in, std::ostream&, NoteMap_t& map)
{
  std::string str;
  if (!(in >> str))
  {
    throw std::logic_error("Not name-note");
  }
  map.erase(str);
}

void muh::link(std::istream& in, std::ostream&, NoteMap_t& map)
{
  std::string from, to;
  if (!(in >> from >> to))
  {
    throw std::logic_error("Not correct arguments");
  }
  for (size_t i = 0; i < map.at(from)->links.size(); ++i)
  {
    if (map.at(from)->links[i].first == to)
    {
      throw std::logic_error("Link already exists");
    }
  }
  map.at(from)->links.emplace_back(to, std::weak_ptr< Note >(map.at(to)));
}

void muh::halt(std::istream& in, std::ostream&, NoteMap_t& map)
{
  std::string from, to;
  if (!(in >> from >> to))
  {
    throw std::logic_error("Not correct arguments");
  }
  auto it = map.at(from)->links.begin();
  for (size_t i = 0; i < map.at(from)->links.size(); ++i)
  {
    if (it->first == to)
    {
      map.at(from)->links.erase(it);
      return;
    }
    ++it;
  }
  throw std::logic_error("Not find to-note");
}

void muh::mind(std::istream& in, std::ostream& out, NoteMap_t& map)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::logic_error("Not name-note");
  }
  if (map.at(name)->links.size() != 0)
  {
    out << map.at(name)->links[0].first;
  }
  for (size_t i = 1; i < map.at(name)->links.size(); ++i)
  {
    if (!(map.at(name)->links[i].second.expired()))
    {
      out << '\n' << map.at(name)->links[i].first;
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
  size_t count = 0;
  for (size_t i = 0; i < map.at(name)->links.size(); ++i)
  {
    if (map.at(name)->links[i].second.expired())
    {
      ++count;
    }
  }
  out << count;
}

void muh::refresh(std::istream& in, std::ostream&, NoteMap_t& map)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::logic_error("Not name-note");
  }
  std::vector< std::pair< std::string, std::weak_ptr< Note > > >& links = map.at(name)->links;
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
