#include "Note.hpp"
#include "DB.hpp"
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
  }

  bool Note::addLink(std::shared_ptr< Note > target)
  {
    for (const auto &wp : links_)
    {
      auto sp = wp.lock();
      if (sp && sp->name_ == target->name_)
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
                                  return sp && sp->name_ == targetName;
                                }),
                 links_.end());
  }

  std::vector< std::string > Note::getLinkedNames() const
  {
    std::vector< std::string > names;
    for (const auto &wp : links_)
    {
      auto sp = wp.lock();
      if (sp)
      {
        names.push_back(sp->name_);
      }
    }
    return names;
  }

  size_t Note::countExpiredLinks() const
  {
    return std::count_if(links_.begin(), links_.end(), [](const auto &wp) {
      return wp.expired();
    });
  }

  void Note::removeExpiredLinks()
  {
    links_.erase(std::remove_if(links_.begin(), links_.end(),
                                [](const auto &wp) {
                                  return wp.expired();
                                }),
                 links_.end());
  }

  const std::string &Note::name() const
  {
    return name_;
  }

  bool NoteDB::createNote(const std::string &name)
  {
    if (notes_.count(name))
    {
      return false;
    }
    notes_[name] = std::make_shared< Note >(name);
    return true;
  }

  bool NoteDB::dropNote(const std::string &name)
  {
    return notes_.erase(name) > 0;
  }

  bool NoteDB::addLineToNote(const std::string &name, const std::string &line)
  {
    auto note = findNote(name);
    if (!note)
    {
      return false;
    }
    note->addLine(line);
    return true;
  }

  bool NoteDB::showNote(const std::string &name, std::ostream &out) const
  {
    auto it = notes_.find(name);
    if (it == notes_.end())
    {
      return false;
    }
    it->second->show(out);
    return true;
  }

  bool NoteDB::linkNotes(const std::string &from, const std::string &to)
  {
    auto fromNote = findNote(from);
    auto toNote = findNote(to);
    if (!fromNote || !toNote)
    {
      return false;
    }
    return fromNote->addLink(toNote);
  }

  bool NoteDB::haltLink(const std::string &from, const std::string &to)
  {
    auto fromNote = findNote(from);
    if (!fromNote)
    {
      return false;
    }
    fromNote->removeLink(to);
    return true;
  }

  std::vector< std::string > NoteDB::mindLinks(const std::string &name) const
  {
    auto it = notes_.find(name);
    if (it == notes_.end())
    {
      return {};
    }
    return it->second->getLinkedNames();
  }

  size_t NoteDB::expiredCount() const
  {
    size_t total = 0;
    for (const auto &pair : notes_)
    {
      total += pair.second->countExpiredLinks();
    }
    return total;
  }

  void NoteDB::refreshAll()
  {
    for (auto &pair : notes_)
    {
      pair.second->removeExpiredLinks();
    }
  }

  bool NoteDB::noteExists(const std::string &name) const
  {
    return notes_.count(name) > 0;
  }

  std::shared_ptr< Note > NoteDB::findNote(const std::string &name)
  {
    auto it = notes_.find(name);
    return (it != notes_.end()) ? it->second : nullptr;
  }

}