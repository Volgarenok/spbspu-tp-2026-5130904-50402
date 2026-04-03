#include "note.hpp"
#include <iostream>

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
