#include "Note.hpp"

samarin::Note::Note(const std::string &name):
  name_(name)
{}

const std::string &samarin::Note::getName() const
{
  return name_;
}
