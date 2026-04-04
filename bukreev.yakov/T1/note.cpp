#include "note.hpp"
#include <iostream>

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
  for (const std::string& str : mLines)
  {
    out << str << '\n';
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
