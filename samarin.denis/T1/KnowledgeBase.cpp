#include "KnowledgeBase.hpp"

#include <ostream>
#include <stdexcept>
#include <vector>

const std::shared_ptr< samarin::Note > &samarin::KnowledgeBase::require(const std::string &name) const
{
  const auto found = notes_.find(name);
  if (found == notes_.end()) {
    throw std::invalid_argument("no such note");
  }
  return found->second;
}

void samarin::KnowledgeBase::note(const std::string &name)
{
  if (notes_.find(name) != notes_.end()) {
    throw std::invalid_argument("note already exists");
  }
  notes_.emplace(name, std::make_shared< Note >(name));
}

void samarin::KnowledgeBase::line(const std::string &name, const std::string &text)
{
  require(name)->addLine(text);
}

void samarin::KnowledgeBase::show(const std::string &name, std::ostream &out) const
{
  const std::shared_ptr< Note > &target = require(name);
  for (const std::string &line: target->getLines()) {
    out << line << '\n';
  }
}

void samarin::KnowledgeBase::drop(const std::string &name)
{
  require(name);
  notes_.erase(name);
}

void samarin::KnowledgeBase::link(const std::string &from, const std::string &to)
{
  const std::shared_ptr< Note > &source = require(from);
  source->link(require(to));
}

void samarin::KnowledgeBase::halt(const std::string &from, const std::string &to)
{
  const std::shared_ptr< Note > &source = require(from);
  source->unlink(require(to));
}

void samarin::KnowledgeBase::mind(const std::string &from, std::ostream &out) const
{
  const std::vector< std::string > names = require(from)->mind();
  for (const std::string &name: names) {
    out << name << '\n';
  }
}
