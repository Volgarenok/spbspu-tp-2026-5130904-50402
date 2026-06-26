#include "commands.hpp"
#include <cstddef>
#include <iomanip>
#include <iterator>
#include <stdexcept>

namespace
{
  void printJoined(std::ostream& out, const std::vector< std::string >& items)
  {
    if (!items.empty())
    {
      out << items.front();
      for (auto it = std::next(items.cbegin()); it != items.cend(); ++it)
      {
        out << "\n" << *it;
      }
    }
  }

  bool searchLoop(const std::shared_ptr< matveev::Note >& start,
    const std::shared_ptr< matveev::Note >& current, std::size_t maxNotes,
    std::vector< std::shared_ptr< matveev::Note > >& path)
  {
    std::size_t intermediates = path.size() - 1;
    for (auto it = current->links.cbegin(); it != current->links.cend(); ++it)
    {
      std::shared_ptr< matveev::Note > next = it->second.lock();
      if (next == nullptr)
      {
        continue;
      }
      if (next == start)
      {
        if (intermediates >= 1)
        {
          return true;
        }
        continue;
      }
      if (intermediates >= maxNotes)
      {
        continue;
      }
      bool onPath = false;
      for (auto visited = path.cbegin(); visited != path.cend(); ++visited)
      {
        if (*visited == next)
        {
          onPath = true;
          break;
        }
      }
      if (onPath)
      {
        continue;
      }
      path.push_back(next);
      if (searchLoop(start, next, maxNotes, path))
      {
        return true;
      }
      path.pop_back();
    }
    return false;
  }
}

void matveev::create_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string name;
  if (!(in >> name))
  {
    return;
  }
  if (db.count(name) != 0)
  {
    throw std::logic_error("note already exists");
  }
  std::shared_ptr< Note > note = std::make_shared< Note >();
  note->name = name;
  db.emplace(name, note);
}

void matveev::add_line(std::istream& in, std::ostream&, db_t& db)
{
  std::string name;
  if (!(in >> name))
  {
    return;
  }
  std::string text;
  if (!(in >> std::quoted(text)))
  {
    return;
  }
  db.at(name)->lines.push_back(text);
}

void matveev::show_note(std::istream& in, std::ostream& out, db_t& db)
{
  std::string name;
  if (!(in >> name))
  {
    return;
  }
  const std::shared_ptr< Note >& note = db.at(name);
  printJoined(out, note->lines);
}

void matveev::drop_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string name;
  if (!(in >> name))
  {
    return;
  }
  if (db.erase(name) == 0)
  {
    throw std::logic_error("note not found");
  }
}

void matveev::link_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string from;
  std::string to;
  if (!(in >> from) || !(in >> to))
  {
    return;
  }
  std::shared_ptr< Note >& noteFrom = db.at(from);
  std::shared_ptr< Note >& noteTo = db.at(to);
  for (auto it = noteFrom->links.cbegin(); it != noteFrom->links.cend(); ++it)
  {
    std::shared_ptr< Note > locked = it->second.lock();
    if (locked != nullptr && locked == noteTo)
    {
      throw std::logic_error("link already exists");
    }
  }
  noteFrom->links.push_back({ to, noteTo });
}

void matveev::mind_note(std::istream& in, std::ostream& out, db_t& db)
{
  std::string name;
  if (!(in >> name))
  {
    return;
  }
  const std::shared_ptr< Note >& note = db.at(name);
  std::vector< std::string > names;
  for (auto it = note->links.cbegin(); it != note->links.cend(); ++it)
  {
    if (!it->second.expired())
    {
      names.push_back(it->first);
    }
  }
  printJoined(out, names);
}

void matveev::halt_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string from;
  std::string to;
  if (!(in >> from) || !(in >> to))
  {
    return;
  }
  std::shared_ptr< Note >& noteFrom = db.at(from);
  if (db.count(to) == 0)
  {
    throw std::logic_error("note not found");
  }
  for (auto it = noteFrom->links.begin(); it != noteFrom->links.end(); ++it)
  {
    if (it->first == to)
    {
      noteFrom->links.erase(it);
      return;
    }
  }
  throw std::logic_error("link not found");
}

void matveev::expired_note(std::istream& in, std::ostream& out, db_t& db)
{
  std::string name;
  if (!(in >> name))
  {
    return;
  }
  const std::shared_ptr< Note >& note = db.at(name);
  std::size_t count = 0;
  for (auto it = note->links.cbegin(); it != note->links.cend(); ++it)
  {
    if (it->second.expired())
    {
      ++count;
    }
  }
  out << count;
}

void matveev::refresh_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string name;
  if (!(in >> name))
  {
    return;
  }
  std::shared_ptr< Note >& note = db.at(name);
  for (auto it = note->links.begin(); it != note->links.end(); )
  {
    if (it->second.expired())
    {
      it = note->links.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

void matveev::loop_note(std::istream& in, std::ostream& out, db_t& db)
{
  std::string from;
  std::size_t maxNotes = 0;
  if (!(in >> from) || !(in >> maxNotes))
  {
    return;
  }
  std::shared_ptr< Note > start = db.at(from);
  std::vector< std::shared_ptr< Note > > path;
  path.push_back(start);
  if (!searchLoop(start, start, maxNotes, path))
  {
    out << "<NO LOOP>";
    return;
  }
  std::vector< std::string > edges;
  for (auto it = path.cbegin(); std::next(it) != path.cend(); ++it)
  {
    edges.push_back((*it)->name + " " + (*std::next(it))->name);
  }
  edges.push_back(path.back()->name + " " + start->name);
  printJoined(out, edges);
}

matveev::cmd_map_t matveev::initCommands()
{
  cmd_map_t cmds;
  cmds["note"] = create_note;
  cmds["line"] = add_line;
  cmds["show"] = show_note;
  cmds["drop"] = drop_note;
  cmds["link"] = link_note;
  cmds["mind"] = mind_note;
  cmds["halt"] = halt_note;
  cmds["expired"] = expired_note;
  cmds["refresh"] = refresh_note;
  cmds["loop"] = loop_note;
  return cmds;
}
