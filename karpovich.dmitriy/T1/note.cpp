#include "note.hpp"
#include <iomanip>
#include <stdexcept>

namespace karpovich
{
  void cmdNote(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Missing name");
    }
    if (name.empty()) {
      throw std::logic_error("Empty name");
    }
    if (notes.find(name) != notes.end()) {
      throw std::logic_error("Note already exists");
    }
    std::shared_ptr< Note > note = std::make_shared< Note >();
    note->name = name;
    notes[name] = note;
  }

  void cmdLine(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string name;
    std::string text;
    if (!(in >> name >> std::quoted(text))) {
      throw std::logic_error("Missing args");
    }
    NoteMap::iterator it = notes.find(name);
    if (it == notes.end()) {
      throw std::logic_error("Note not found");
    }
    it->second->lines.push_back(text);
  }

  void cmdShow(std::istream &in, std::ostream &out, NoteMap &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Missing name");
    }
    NoteMap::const_iterator it = notes.find(name);
    if (it == notes.end()) {
      throw std::logic_error("Note not found");
    }
    if (it->second->lines.size() == 0) {
      out << '\n';
    }
    for (size_t i = 0; i < it->second->lines.size(); ++i) {
      out << it->second->lines[i] << '\n';
    }
  }

  void cmdDrop(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Missing name");
    }
    NoteMap::iterator it = notes.find(name);
    if (it == notes.end()) {
      throw std::logic_error("Note not found");
    }
    notes.erase(it);
  }

  void cmdLink(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string from_name;
    std::string to_name;
    if (!(in >> from_name >> to_name)) {
      throw std::logic_error("Missing args");
    }
    NoteMap::iterator it_from = notes.find(from_name);
    NoteMap::iterator it_to = notes.find(to_name);
    if (it_from == notes.end() || it_to == notes.end()) {
      throw std::logic_error("Note not found");
    }
    for (size_t i = 0; i < it_from->second->links.size(); ++i) {
      if (it_from->second->links[i].first == to_name) {
        throw std::logic_error("Link already exists");
      }
    }
    it_from->second->links.emplace_back(to_name, std::weak_ptr< Note >(it_to->second));
  }

  void cmdHalt(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string from_name;
    std::string to_name;
    if (!(in >> from_name >> to_name)) {
      throw std::logic_error("Missing args");
    }
    NoteMap::iterator it_from = notes.find(from_name);
    NoteMap::iterator it_to = notes.find(to_name);
    if (it_from == notes.end() || it_to == notes.end()) {
      throw std::logic_error("Note not found");
    }
    bool found = false;
    for (size_t i = 0; i < it_from->second->links.size(); ++i) {
      if (it_from->second->links[i].first == to_name) {
        it_from->second->links.erase(it_from->second->links.begin() + static_cast< long >(i));
        found = true;
        break;
      }
    }
    if (!found) {
      throw std::logic_error("Link not found");
    }
  }

  void cmdMind(std::istream &in, std::ostream &out, NoteMap &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Missing name");
    }
    NoteMap::const_iterator it = notes.find(name);
    if (it == notes.end()) {
      throw std::logic_error("Note not found");
    }
    bool printed = false;
    for (size_t i = 0; i < it->second->links.size(); ++i) {
      if (!it->second->links[i].second.expired()) {
        out << it->second->links[i].first << '\n';
        printed = true;
      }
    }
    if (!printed) {
      out << '\n';
    }
  }

  void cmdExpired(std::istream &in, std::ostream &out, NoteMap &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Missing name");
    }
    NoteMap::const_iterator it = notes.find(name);
    if (it == notes.end()) {
      throw std::logic_error("Note not found");
    }
    size_t count = 0;
    for (size_t i = 0; i < it->second->links.size(); ++i) {
      if (it->second->links[i].second.expired()) {
        count = count + 1;
      }
    }
    out << count << '\n';
  }

  void cmdRefresh(std::istream &in, std::ostream &, NoteMap &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Missing name");
    }
    NoteMap::iterator it = notes.find(name);
    if (it == notes.end()) {
      throw std::logic_error("Note not found");
    }
    std::vector< std::pair< std::string, std::weak_ptr< Note > > > &links = it->second->links;
    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < links.size(); ++read_idx) {
      if (!links[read_idx].second.expired()) {
        if (write_idx != read_idx) {
          links[write_idx] = links[read_idx];
        }
        write_idx = write_idx + 1;
      }
    }
    links.resize(write_idx);
  }
}
