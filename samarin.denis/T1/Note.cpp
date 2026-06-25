#include "Note.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>

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

samarin::Note::LinkList::const_iterator samarin::Note::findLink(const std::shared_ptr< Note > &target) const
{
  const auto matches = [&target](const std::weak_ptr< Note > &link)
  {
    return link.lock() == target;
  };
  return std::find_if(links_.begin(), links_.end(), matches);
}

void samarin::Note::link(const std::shared_ptr< Note > &target)
{
  if (findLink(target) != links_.end()) {
    throw std::invalid_argument("link already exists");
  }
  links_.push_back(target);
}

void samarin::Note::unlink(const std::shared_ptr< Note > &target)
{
  const auto found = findLink(target);
  if (found == links_.end()) {
    throw std::invalid_argument("no such link");
  }
  links_.erase(found);
}

std::vector< std::string > samarin::Note::mind() const
{
  std::vector< std::string > names;
  for (const std::weak_ptr< Note > &link: links_) {
    const std::shared_ptr< Note > target = link.lock();
    if (target) {
      names.push_back(target->getName());
    }
  }
  return names;
}

std::size_t samarin::Note::countExpired() const
{
  return std::count_if(links_.begin(), links_.end(), std::mem_fn(&std::weak_ptr< Note >::expired));
}
