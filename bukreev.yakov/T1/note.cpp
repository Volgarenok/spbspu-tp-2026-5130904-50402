#include "note.hpp"
#include <iostream>

bukreev::Note::Note(std::string name):
  mName(name)
{}

std::string bukreev::Note::getName()
{
  return mName;
}

void bukreev::Note::appendLine(std::string line)
{
  mLines.push_back(line);
}

void bukreev::Note::show(std::ostream& out)
{
  if (mLines.size() == 0)
  {
    out << '\n';
  }
  else
  {
    out << mLines[0];

    for (size_t i = 1; i < mLines.size(); i++)
    {
      out << '\n' << mLines[i];
    }
  }
}

void bukreev::Note::addLink(std::shared_ptr< Note > to)
{
  for (const NoteLink& link : mLinks)
  {
    if (link.first == to->getName())
    {
      throw std::logic_error("Link already exists");
    }
  }
  mLinks.push_back({to->getName(), to});
}

void bukreev::Note::removeLink(std::string to)
{
  for (size_t i = 0; i < mLinks.size(); i++)
  {
    if (mLinks[i].first == to)
    {
      mLinks.erase(mLinks.begin() + i);
      break;
    }
  }
}

void bukreev::Note::showLinks(std::ostream& out)
{
  bool shown = false;
  for (const NoteLink& link : mLinks)
  {
    if (!link.second.expired())
    {
      if (shown)
      {
        out << '\n' << link.first;
      }
      else
      {
        out << link.first;
        shown = true;
      }
    }
  }
}

size_t bukreev::Note::countExpired()
{
  size_t res = 0;
  for (const NoteLink& link : mLinks)
  {
    if (link.second.expired())
    {
      res++;
    }
  }

  return res;
}

void bukreev::Note::removeExpired()
{
  std::vector< std::string > expired;

  for (const NoteLink& link : mLinks)
  {
    if (link.second.expired())
    {
      expired.push_back(link.first);
    }
  }

  for (const std::string& name : expired)
  {
    removeLink(name);
  }
}
