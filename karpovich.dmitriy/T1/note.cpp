#include "note.hpp"
#include <cstddef>
#include <iomanip>
#include <stdexcept>
#include <string>

namespace
{
  using str_t = std::string;
  using mp_t = karpovich::NoteMap;
  using vstr_t = std::vector< str_t >;
  bool findLoop(const str_t &startName, const str_t &currentName, size_t depthLeft, mp_t &notes, vstr_t &path)
  {
    if (depthLeft == 0) {
      return false;
    }
    const std::shared_ptr< karpovich::Note > &current = notes.at(currentName);
    for (size_t i = 0; i < current->links.size(); ++i) {
      const str_t &nextName = current->links[i].first;
      if (current->links[i].second.expired()) {
        continue;
      }
      path.push_back(nextName);
      if (nextName == startName) {
        return true;
      }
      if (findLoop(startName, nextName, depthLeft - 1, notes, path)) {
        return true;
      }
      path.pop_back();
    }
    return false;
  }
}

karpovich::Note::Note(const std::string &name):
  name(name),
  lines(),
  links()
{}

void karpovich::cmdNote(std::istream &in, std::ostream &, NoteMap &notes)
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
  std::shared_ptr< Note > note = std::make_shared< Note >(name);
  notes[name] = note;
}

void karpovich::cmdLine(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  std::string text;
  if (!(in >> name >> std::quoted(text))) {
    throw std::logic_error("Missing args");
  }
  notes.at(name)->lines.push_back(text);
}

void karpovich::cmdShow(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("Missing name");
  }
  const std::vector< std::string > &lines = notes.at(name)->lines;
  if (lines.empty()) {
    return;
  }
  out << lines[0];
  for (size_t i = 1; i < lines.size(); ++i) {
    out << '\n' << lines[i];
  }
}

void karpovich::cmdDrop(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("Missing name");
  }
  notes.at(name);
  notes.erase(name);
}

void karpovich::cmdLink(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string from_name;
  std::string to_name;
  if (!(in >> from_name >> to_name)) {
    throw std::logic_error("Missing args");
  }
  std::shared_ptr< Note > from = notes.at(from_name);
  std::shared_ptr< Note > to = notes.at(to_name);
  for (size_t i = 0; i < from->links.size(); ++i) {
    if (from->links[i].first == to_name) {
      throw std::logic_error("Link already exists");
    }
  }
  from->links.emplace_back(to_name, std::weak_ptr< Note >(to));
}

void karpovich::cmdHalt(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string from_name;
  std::string to_name;
  if (!(in >> from_name >> to_name)) {
    throw std::logic_error("Missing args");
  }
  std::shared_ptr< Note > from = notes.at(from_name);
  notes.at(to_name);
  std::vector< std::pair< std::string, std::weak_ptr< Note > > >::iterator it = from->links.begin();
  while (it != from->links.end() && it->first != to_name) {
    ++it;
  }
  if (it == from->links.end()) {
    throw std::logic_error("Link not found");
  }
  from->links.erase(it);
}

void karpovich::cmdMind(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("Missing name");
  }
  const std::vector< std::pair< std::string, std::weak_ptr< Note > > > &links = notes.at(name)->links;
  for (size_t i = 0; i < links.size(); ++i) {
    if (!links[i].second.expired()) {
      out << links[i].first;
      for (size_t j = i + 1; j < links.size(); ++j) {
        if (!links[j].second.expired()) {
          out << '\n' << links[j].first;
        }
      }
      return;
    }
  }
}

void karpovich::cmdExpired(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("Missing name");
  }
  size_t count = 0;
  const std::vector< std::pair< std::string, std::weak_ptr< Note > > > &links = notes.at(name)->links;
  for (size_t i = 0; i < links.size(); ++i) {
    if (links[i].second.expired()) {
      count = count + 1;
    }
  }
  out << count;
}

void karpovich::cmdRefresh(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("Missing name");
  }
  std::vector< std::pair< std::string, std::weak_ptr< Note > > > &links = notes.at(name)->links;
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

void karpovich::cmdLoop(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  size_t n = 0;
  if (!(in >> name >> n)) {
    throw std::logic_error("Invalid args");
  }
  notes.at(name);
  std::vector< std::string > path;
  path.push_back(name);
  if (!findLoop(name, name, n + 1, notes, path)) {
    out << "<NO LOOP>";
    return;
  }
  for (size_t i = 0; i < path.size() - 1; ++i) {
    out << path[i] << ' ' << path[i + 1];
    if (i < path.size() - 2) {
      out << '\n';
    }
  }
}
