#include "commands.hpp"

#include <algorithm>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

void sogdanov::cmd_note(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  if (notes.find(name) != notes.end()) {
    throw std::logic_error("note already exists");
  }
  notes.emplace(name, std::make_shared< Note >(name));
}

void sogdanov::cmd_line(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  std::string text;
  if (!(in >> name >> std::quoted(text))) {
    throw std::logic_error("no arguments");
  }
  notes.at(name)->lines.push_back(text);
}

void sogdanov::cmd_show(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  NotePtr note = notes.at(name);
  if (!note->lines.empty()) {
    out << note->lines[0];
    for (size_t i = 1; i < note->lines.size(); ++i) {
      out << "\n" << note->lines[i];
    }
  }
}

void sogdanov::cmd_drop(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  notes.at(name);
  notes.erase(name);
}

void sogdanov::cmd_link(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string from;
  std::string to;
  if (!(in >> from >> to)) {
    throw std::logic_error("no arguments");
  }
  NotePtr note_from = notes.at(from);
  NotePtr note_to = notes.at(to);
  for (const std::weak_ptr< Note > &w : note_from->links) {
    if (w.lock() == note_to) {
      throw std::logic_error("link already exists");
    }
  }
  note_from->links.push_back(note_to);
}

void sogdanov::cmd_halt(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string from;
  std::string to;
  if (!(in >> from >> to)) {
    throw std::logic_error("no arguments");
  }
  NotePtr note_from = notes.at(from);
  std::vector< std::weak_ptr< Note > > &links = note_from->links;
  for (auto it = links.begin(); it != links.end(); ++it) {
    NotePtr target = it->lock();
    if (target && target->name == to) {
      links.erase(it);
      return;
    }
  }
  throw std::logic_error("link not found");
}

void sogdanov::cmd_mind(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  NotePtr note = notes.at(name);
  std::vector< NotePtr > valid_links;
  for (const std::weak_ptr< Note > &w : note->links) {
    if (NotePtr target = w.lock()) {
      valid_links.push_back(target);
    }
  }
  if (!valid_links.empty()) {
    out << valid_links[0]->name;
    for (size_t i = 1; i < valid_links.size(); ++i) {
      out << "\n" << valid_links[i]->name;
    }
  }
}

void sogdanov::cmd_expired(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  NotePtr note = notes.at(name);
  int count = 0;
  for (const std::weak_ptr< Note > &w : note->links) {
    if (w.expired()) {
      ++count;
    }
  }
  out << count;
}

void sogdanov::cmd_refresh(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  if (!(in >> name)) {
    throw std::logic_error("no name");
  }
  NotePtr note = notes.at(name);
  std::vector< std::weak_ptr< Note > > &links = note->links;
  for (auto i = links.begin(); i != links.end();) {
    if (i->expired()) {
      i = links.erase(i);
    } else {
      ++i;
    }
  }
}

bool sogdanov::find_loop_dfs(NotePtr current,
    NotePtr start, int max_edges, int current_edges, std::vector< NotePtr > &path)
{
  if (current_edges > 0 && current == start) {
    return true;
  }
  if (current_edges == max_edges) {
    return false;
  }

  for (const std::weak_ptr< sogdanov::Note > &w : current->links) {
    NotePtr next = w.lock();
    if (!next) {
      continue;
    }

    if (next != start) {
      if (std::find(path.begin(), path.end(), next) != path.end()) {
        continue;
      }
    }

    path.push_back(next);

    if (sogdanov::find_loop_dfs(next, start, max_edges, current_edges + 1, path)) {
      return true;
    }

    path.pop_back();
  }

  return false;
}

void sogdanov::cmd_loop(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  int n;
  if (!(in >> name >> n)) {
    throw std::logic_error("no arguments");
  }

  NotePtr start_note = notes.at(name);
  std::vector< NotePtr > path;

  if (find_loop_dfs(start_note, start_note, n + 1, 0, path)) {
    NotePtr prev = start_note;
    for (size_t i = 0; i < path.size(); ++i) {
      if (i > 0) {
        out << "\n";
      }
      out << prev->name << " " << path[i]->name;
      prev = path[i];
    }
  } else {
    out << "<NO LOOP>\n";
  }
}
