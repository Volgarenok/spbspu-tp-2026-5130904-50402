#include "db.hpp"
#include "note.hpp"
#include <algorithm>

namespace chernikov {

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
