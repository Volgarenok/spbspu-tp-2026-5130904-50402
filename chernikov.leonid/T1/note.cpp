#include "note.hpp"
#include <algorithm>

namespace chernikov {

  Note::Note(std::string name):
    name_(std::move(name))
  {
  }

  void Note::addLine(const std::string &text)
  {
    lines_.push_back(text);
  }

  void Note::show(std::ostream &out) const
  {
    for (const auto &line : lines_)
    {
      out << line << '\n';
    }
    if (lines_.empty())
    {
      out << '\n';
    }
  }

  bool Note::addLink(std::shared_ptr< Note > target)
  {
    for (const auto &link : links_)
    {
      auto existing = link.lock();
      if (existing && existing.get() == target.get())
      {
        return false;
      }
    }
    links_.push_back(target);
    return true;
  }

  void Note::removeLink(const std::string &targetName)
  {
    links_.erase(std::remove_if(links_.begin(), links_.end(),
                                [&targetName](const std::weak_ptr< Note > &wp) {
                                  auto sp = wp.lock();
                                  return !sp || sp->name() == targetName;
                                }),
                 links_.end());
  }

  std::vector< std::string > Note::getLinkedNames() const
  {
    std::vector< std::string > names;
    for (const auto &link : links_)
    {
      auto sp = link.lock();
      if (sp)
      {
        names.push_back(sp->name());
      }
    }
    return names;
  }

  size_t Note::countExpiredLinks() const
  {
    return std::count_if(links_.begin(), links_.end(), [](const std::weak_ptr< Note > &wp) {
      return wp.expired();
    });
  }

  void Note::removeExpiredLinks()
  {
    links_.erase(std::remove_if(links_.begin(), links_.end(),
                                [](const std::weak_ptr< Note > &wp) {
                                  return wp.expired();
                                }),
                 links_.end());
  }

  const std::string &Note::name() const
  {
    return name_;
  }

}
