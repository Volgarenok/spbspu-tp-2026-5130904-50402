#include "command.hpp"
#include <iostream>
#include <iomanip>
#include "note.hpp"

void afanasev::noteCmd(std::istream & in, std::ostream &, note_t & db)
{
  std::string name;
  in >> name;

  std::pair< note_t::iterator, bool > result = db.emplace(name, std::make_shared< Note >());
  if (!result.second)
  {
    throw std::out_of_range("This note have");
  }
}

void afanasev::lineCmd(std::istream & in, std::ostream &, note_t & db)
{
  std::string name, text;
  in >> name;

  in >> std::quoted(text);

  db.at(name)->text.push_back(text);
}

void afanasev::showCmd(std::istream & in, std::ostream & out, note_t & db)
{
  std::string name;
  in >> name;

  std::shared_ptr< Note > note = db.at(name);
  const std::vector< std::string > & lines = note->text;
  has_output = true;
  if (!lines.empty())
  {
    for (size_t i = 0; i < lines.size(); ++i)
    {
      out << lines[i];
      if (i + 1 < lines.size())
      {
        out << '\n';
      }
    }
  }
}

void afanasev::dropCmd(std::istream & in, std::ostream &, note_t & db)
{
  std::string name;
  in >> name;

  if (db.find(name) == db.end())
  {
    throw std::out_of_range("Note does not exist");
  }

  db.erase(name);
}

void afanasev::linkCmd(std::istream & in, std::ostream &, note_t & db)
{
  std::string name, link;
  in >> name >> link;

  std::shared_ptr< Note > src = db.at(name);
  std::shared_ptr< Note > dst = db.at(link);

  for (const std::pair< std::string, std::weak_ptr< Note > > & ptr : src->ptr)
  {
    if (ptr.first == link && !ptr.second.expired())
    {
      throw std::out_of_range("This link already exists");
    }
  }

  src->ptr.push_back({link, std::weak_ptr< Note >(dst)});
}

void afanasev::haltCmd(std::istream & in, std::ostream &, note_t & db)
{
  std::string name, link;
  in >> name >> link;

  std::shared_ptr< Note > src = db.at(name);
  std::vector< std::pair< std::string, std::weak_ptr< Note > > >::iterator it = src->ptr.begin();

  while (it != src->ptr.end() && it->first != link)
  {
    ++it;
  }

  if (it == src->ptr.end() || it->second.expired())
  {
    throw std::out_of_range("Link not found or expired");
  }

  src->ptr.erase(it);
}

void afanasev::mindCmd(std::istream & in, std::ostream & out, note_t & db)
{
  std::string name;
  in >> name;

  std::shared_ptr< Note > note = db.at(name);
  const std::vector< std::pair< std::string, std::weak_ptr< Note > > > & links = note->ptr;

  has_output = true;
  bool first = true;
  for (const std::pair< std::string, std::weak_ptr< Note > > & link : links)
  {
    if (!link.second.expired())
    {
      if (!first)
      {
        out << '\n';
      }
      first = false;
      out << link.first;
    }
  }
}

void afanasev::expiredCmd(std::istream & in, std::ostream & out, note_t & db)
{
  std::string name;
  in >> name;

  std::shared_ptr< Note > note = db.at(name);
  size_t cnt = 0;
  for (const std::pair< std::string, std::weak_ptr< Note > > & ptr : note->ptr)
  {
    if (ptr.second.expired())
    {
      ++cnt;
    }
  }
  out << cnt;
  has_output = true;
}

void afanasev::refreshCmd(std::istream & in, std::ostream &, note_t & db)
{
  std::string name;
  in >> name;

  std::vector< std::pair< std::string, std::weak_ptr< Note > > > & links = db.at(name)->ptr;

  std::vector< std::pair< std::string, std::weak_ptr< Note > > >::iterator it = links.begin();

  while (it != links.end())
  {
    if (it->second.expired())
    {
      it = links.erase(it);
    }
    else
    {
      ++it;
    }
  }
}
