#include "commands.hpp"
#include <iomanip>
#include <stdexcept>

void novikov::note(std::istream& in, std::ostream&, novikov::map_t& notes)
{
  std::string noteName;
  in >> noteName;
  if (notes.find(noteName) == notes.end())
  {
    notes.insert({noteName, std::make_shared< Note >()});
  }
  else
  {
    throw std::logic_error("");
  }
}

void novikov::line(std::istream& in, std::ostream&, novikov::map_t& notes)
{
  std::string noteName, quotedText;
  in >> noteName >> std::quoted(quotedText);
  notes.at(noteName)->entries.push_back(quotedText);
}

void novikov::show(std::istream& in, std::ostream& out, novikov::map_t& notes)
{
  std::string noteName;
  in >> noteName;
  std::vector< std::string > entries = notes.at(noteName)->entries;
  if (!entries.empty())
  {
    out << entries[0];
  }
  for (std::string line : entries)
  {
    out << '\n' << line;
  }
}

void novikov::drop(std::istream& in, std::ostream&, novikov::map_t& notes)
{
  std::string noteName;
  in >> noteName;
  notes.at(noteName);
  notes.erase(noteName);
}

void novikov::link(std::istream& in, std::ostream&, novikov::map_t& notes)
{
  std::string noteTo, noteFrom;
  in >> noteFrom >> noteTo;
  std::shared_ptr< Note > toPtr = notes.at(noteTo);
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  if (!exist(fromPtr->links, noteTo))
  {
    fromPtr->links.push_back({noteTo, toPtr});
  }
  else
  {
    throw std::logic_error("");
  }
}

void novikov::halt(std::istream& in, std::ostream&, novikov::map_t& notes)
{
  std::string noteTo, noteFrom;
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

void novikov::mind(std::istream& in, std::ostream& out, novikov::map_t& notes)
{
  std::string noteFrom;
  in >> noteFrom;
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  std::vector< std::string > linkIds;
  for (const auto& pair : fromPtr->links)
  {
    if (pair.second.lock())
    {
      linkIds.push_back(pair.first);
    }
  }
  if (!linkIds.empty())
  {
    out << linkIds[0];
  }
  for (const std::string& id : linkIds)
  {
    out << "\n" << id;
  }
}

void novikov::expired(std::istream& in, std::ostream& out, novikov::map_t& notes)
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

void novikov::refresh(std::istream& in, std::ostream&, novikov::map_t& notes)
{
  std::string noteFrom;
  in >> noteFrom;
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  novikov::links_t newLinksVec;
  for (auto pair : fromPtr->links)
  {
    if (pair.second.lock())
    {
      newLinksVec.push_back(pair);
    }
  }
  fromPtr->links = newLinksVec;
}

bool novikov::exist(const novikov::links_t& v, const std::string& s)
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
