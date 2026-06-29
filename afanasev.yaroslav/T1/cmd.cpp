#include <iostream>
#include <iomanip>
#include "cmd.hpp"
#include "note.hpp"

void afanasev::noteCmd(std::istream & in, std::ostream &, note_t & db)
{
  std::string name;
  in >> name;

  if (db.find(name) == db.end())
  {
    db[name] = std::make_shared< Note >();
  }
  else
  {
    throw std::out_of_range("This note have");
  }
}

void afanasev::lineCmd(std::istream & in, std::ostream &, note_t & db)
{
  std::string name, text;
  in >> name;

  in >> std::quoted(text);

  db.at(name)->text_.push_back(text);
}

void afanasev::showCmd(std::istream & in, std::ostream & out, note_t & db)
{
  std::string name;
  in >> name;

  bool outp = false;

  for (const std::string & line : db.at(name)->text_)
  {
    out << line << '\n';
    outp = true;
  }

  if (!outp)
  {
    out << '\n';
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

  for (const std::pair< std::string, std::weak_ptr< Note > > & ptr : src->ptr_)
  {
    if (ptr.first == link)
    {
      throw std::out_of_range("This link already exists");
    }
  }

  src->ptr_.push_back({link, dst});
}

void afanasev::haltCmd(std::istream & in, std::ostream &, note_t & db)
{
  std::string name, link;
  in >> name >> link;

  std::shared_ptr< Note > src = db.at(name);
  std::vector< std::pair< std::string, std::weak_ptr< Note > > >::iterator it = src->ptr_.begin();

  while (it != src->ptr_.end() && it->first != link)
  {
    ++it;
  }

  if (it == src->ptr_.end() || it->second.expired())
  {
    throw std::out_of_range("Link not found or expired");
  }

  src->ptr_.erase(it);
}

void afanasev::mindCmd(std::istream & in, std::ostream & out, note_t & db)
{
  std::string name;
  in >> name;

  bool outp = false;

  for (const std::pair< std::string, std::weak_ptr< Note > > & ptr : db.at(name)->ptr_)
  {
    if (!ptr.second.expired())
    {
      out << ptr.first << '\n';
      outp = true;
    }
  }

  if (!outp)
  {
    out << '\n';
  }
}

void afanasev::expiredCmd(std::istream & in, std::ostream & out, note_t & db)
{
  size_t cnt = 0;
  std::string name;
  in >> name;

  for (const std::pair< std::string, std::weak_ptr< Note > > & ptr : db.at(name)->ptr_)
  {
    if (ptr.second.expired())
    {
      ++cnt;
    }
  }

  out << cnt << '\n';
}

void afanasev::refreshCmd(std::istream & in, std::ostream &, note_t & db)
{
  std::string name;
  in >> name;

  std::vector< std::pair< std::string, std::weak_ptr< Note > > > & links = db.at(name)->ptr_;

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
