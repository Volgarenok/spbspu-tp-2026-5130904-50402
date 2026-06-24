#include "Note.hpp"

samarin::Note::Note(const std::string &name):
  name_(name)
{}

const std::string &samarin::Note::getName() const
{
  return name_;
}

void samarin::Note::addLine(const std::string &line)
{
  lines_.push_back(line);
}

const std::vector< std::string > &samarin::Note::getLines() const
{
  return lines_;
}
