#include "commands.hpp"
#include <iomanip>
#include <stdexcept>
#include <vector>

void shirokov::note(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteName;
  in >> noteName;
  if (notes.find(noteName) == notes.end())
  {
    notes.insert({noteName, std::make_shared< Note >()});
  }
  else
  {
    throw std::logic_error("A note with this name already exists.");
  }
}

void shirokov::line(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteName, quotedText;
  in >> noteName >> std::quoted(quotedText);
  notes.at(noteName)->entries.push_back(quotedText);
}

void shirokov::show(std::istream& in, std::ostream& out, map_t& notes)
{
  std::string noteName;
  in >> noteName;
  std::vector< std::string > entries = notes.at(noteName)->entries;
  if (!entries.empty())
  {
    out << entries[0];
  }
  for (size_t i = 1; i < entries.size(); ++i)
  {
    out << '\n' << entries[i];
  }
}

void shirokov::drop(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteName;
  in >> noteName;
  notes.at(noteName);
  notes.erase(noteName);
}

void shirokov::link(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteFrom, noteTo;
  in >> noteFrom >> noteTo;
  std::shared_ptr< Note > toPtr = notes.at(noteTo);
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  if (!exist(fromPtr->links, noteTo))
  {
    fromPtr->links.push_back({noteTo, toPtr});
  }
  else
  {
    throw std::logic_error("The link to this note already exists");
  }
}

void shirokov::halt(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteFrom, noteTo;
  in >> noteFrom >> noteTo;
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  notes.at(noteTo);
  for (auto it = fromPtr->links.begin(); it != fromPtr->links.end(); ++it)
  {
    if (it->first == noteTo)
    {
      fromPtr->links.erase(it);
      break;
    }
  }
}

void shirokov::mind(std::istream& in, std::ostream& out, map_t& notes)
{
  std::string noteFrom;
  in >> noteFrom;
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  std::vector< std::string > ids;
  for (const auto& pair : fromPtr->links)
  {
    if (pair.second.lock())
    {
      ids.push_back(pair.first);
    }
  }
  if (!ids.empty())
  {
    out << ids[0];
  }
  for (size_t i = 1; i < ids.size(); ++i)
  {
    out << '\n' << ids[i];
  }
}

void shirokov::expired(std::istream& in, std::ostream& out, map_t& notes)
{
  std::string noteFrom;
  in >> noteFrom;
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  size_t k = 0;
  for (const auto& pair : fromPtr->links)
  {
    if (!pair.second.lock())
    {
      ++k;
    }
  }
  out << k;
}

void shirokov::refresh(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteFrom;
  in >> noteFrom;
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  links_t newLinksVec;
  for (auto pair : fromPtr->links)
  {
    if (pair.second.lock())
    {
      newLinksVec.push_back(pair);
    }
  }
  fromPtr->links = newLinksVec;
}

bool shirokov::exist(const links_t& v, const std::string& s)
{
  for (size_t i = 0; i < v.size(); ++i)
  {
    if (v[i].first == s)
    {
      return true;
    }
  }
  return false;
}
