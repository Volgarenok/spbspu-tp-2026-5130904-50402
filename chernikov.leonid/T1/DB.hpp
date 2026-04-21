#ifndef NOTE_HPP
#define NOTE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

namespace chernikov {

  class Note;
  class NoteDB
  {
  public:
    bool createNote(const std::string &name);
    bool dropNote(const std::string &name);
    bool addLineToNote(const std::string &name, const std::string &line);
    bool showNote(const std::string &name, std::ostream &out) const;
    bool linkNotes(const std::string &from, const std::string &to);
    bool haltLink(const std::string &from, const std::string &to);
    std::vector< std::string > mindLinks(const std::string &name) const;
    size_t expiredCount() const;
    void refreshAll();
    bool noteExists(const std::string &name) const;

  private:
    std::unordered_map< std::string, std::shared_ptr< Note > > notes_;

    std::shared_ptr< Note > findNote(const std::string &name);
  };

}

#endif