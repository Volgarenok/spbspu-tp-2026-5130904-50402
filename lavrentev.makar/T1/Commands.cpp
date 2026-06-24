#include "Commands.hpp"
#include <memory>
#include <stdexcept>
#include <iomanip>
#include <unordered_map>

void lavrentev::note(std::istream &in, std::ostream &, dbTemp &db)
{
  std::string newName;
  in >> newName;
  try
  {
    db.at(newName);
    throw std::logic_error("Node is already exists");
  }
  catch (const std::out_of_range &)
  {
    std::shared_ptr< Note > newNote = std::make_shared< Note >(newName);
    db.insert({newName, newNote});
  }
}

void lavrentev::line(std::istream &in,
  std::ostream &,
  std::unordered_map< std::string, std::shared_ptr< Note > > &db)
{
  std::string name, text;
  in >> name;
  try
  {
    std::shared_ptr< Note > notePtr = db.at(name);
    in >> std::quoted(text);
    notePtr->lines.push_back(text);
  }
  catch (const std::out_of_range &)
  {
    throw std::logic_error("No such Note");
  }
}

void lavrentev::show(std::istream &in,
  std::ostream &out,
  std::unordered_map< std::string, std::shared_ptr< Note > > &db)
{
  std::string name;
  in >> name;
  try
  {
    std::shared_ptr< Note > notePtr = db.at(name);
    if (notePtr->lines.empty())
    {
      return;
    }

    size_t i = 0;
    out << notePtr->lines[i];
    for (i = 1; i < notePtr->lines.size(); ++i)
    {
      out << "\n" << notePtr->lines[i];
    }
  }
  catch (const std::out_of_range &)
  {
    throw std::logic_error("No such Note");
  }
}

void lavrentev::drop(std::istream &in,
  std::ostream &,
  std::unordered_map< std::string, std::shared_ptr< Note > > &db)
{
  std::string name;
  in >> name;
  try
  {
    db.at(name);
    db.erase(name);
  }
  catch (const std::out_of_range &)
  {
    throw std::logic_error("No such Note");
  }
}

void lavrentev::link(std::istream &in,
  std::ostream &,
  std::unordered_map< std::string, std::shared_ptr< Note > > &db)
{
  std::string noteTo, noteFrom;
  in >> noteFrom >> noteTo;
  try
  {
    std::shared_ptr< Note > fromPtr = db.at(noteFrom);
    std::shared_ptr< Note > toPtr = db.at(noteTo);

    for (size_t i = 0; i < fromPtr->ptrs.size(); ++i)
    {
      std::shared_ptr< Note > k = fromPtr->ptrs[i].lock();
      if (k != nullptr)
      {
        if (k->name == noteTo)
        {
          throw std::logic_error("Link is already exists");
        }
      }
    }
    std::weak_ptr< Note > newLink = toPtr;
    fromPtr->ptrs.push_back(newLink);
  }
  catch (const std::out_of_range &)
  {
    throw std::logic_error("No such Note");
  }
}

void lavrentev::mind(std::istream &in,
  std::ostream &out,
  std::unordered_map< std::string, std::shared_ptr< Note > > &db)
{
  std::string name;
  in >> name;
  try
  {
    std::shared_ptr< Note > notePtr = db.at(name);
    if (notePtr->ptrs.empty())
    {
      return;
    }

    size_t i = 0;
    for (; i < notePtr->ptrs.size(); ++i)
    {
      std::shared_ptr< Note > firstLink = notePtr->ptrs[i].lock();
      if (firstLink != nullptr)
      {
        out << firstLink->name;
        break;
      }
    }

    ++i;
    for (; i < notePtr->ptrs.size(); ++i)
    {
      std::shared_ptr< Note > k = notePtr->ptrs[i].lock();
      if (k != nullptr)
      {
        out << "\n" << k->name;
      }
    }
  }
  catch (const std::out_of_range &)
  {
    throw std::logic_error("No such Note");
  }
}

void lavrentev::halt(std::istream &in,
  std::ostream &,
  std::unordered_map< std::string, std::shared_ptr< Note > > &db)
{
  std::string noteTo, noteFrom;
  in >> noteFrom >> noteTo;
  try
  {
    db.at(noteTo);
    std::shared_ptr< Note > fromPtr = db.at(noteFrom);
    
    std::vector< std::weak_ptr< lavrentev::Note > > &ptrs = fromPtr->ptrs;
    for (size_t i = 0; i < ptrs.size(); ++i)
    {
      std::shared_ptr< Note > k = ptrs[i].lock();
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
  catch (const std::out_of_range &)
  {
    throw std::logic_error("No such Note");
  }
}

void lavrentev::expired(std::istream &in, std::ostream &out, dbTemp &db)
{
  std::string name;
  in >> name;
  try
  {
    std::shared_ptr< Note > notePtr = db.at(name);
    size_t count = 0;
    for (std::weak_ptr< Note > wp : notePtr->ptrs)
    {
      if (wp.expired())
      {
        ++count;
      }
    }
    out << count;
  }
  catch (const std::out_of_range &)
  {
    throw std::logic_error("No such Note");
  }
}

void lavrentev::refresh(std::istream &in, std::ostream &, dbTemp &db)
{
  std::string name;
  in >> name;
  try
  {
    std::shared_ptr< Note > notePtr = db.at(name);
    for (std::vector< std::weak_ptr< Note > >::iterator it = notePtr->ptrs.begin(); it != notePtr->ptrs.end();)
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
  catch (const std::out_of_range &)
  {
    throw std::logic_error("No such Note");
  }
}
