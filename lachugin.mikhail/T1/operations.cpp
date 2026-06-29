#include "operations.hpp"
#include <vector>
#include <iomanip>
#include <string>
#include <unordered_map>
#include "note.hpp"

void lachugin::addNote(std::istream& in, std::ostream &, notesMap &db)
{
  std::string note;
  in >> note;
  auto it = db.find(note);
  if (it != db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }
  db.insert({note, std::make_shared< Note >(note)});
}

void lachugin::addLine(std::istream &in, std::ostream &, notesMap &db)
{
  std::string note;
  in >> note;

  std::string str;
  in >> std::quoted(str);
  db.at(note)->lines.push_back(str);
}

void lachugin::showNote(std::istream& in, std::ostream& out, notesMap& db)
{
  std::string note;
  in >> note;

  auto notePtr = db.at(note);
  if (notePtr->lines.empty())
  {
    return;
  }

  out << notePtr->lines.front();
  for (size_t i = 1; i < notePtr->lines.size(); ++i)
  {
    out << '\n' << notePtr->lines[i];
  }
}


void lachugin::addLink(std::istream &in, std::ostream &, notesMap &db)
{
  std::string note1, note2;
  in >> note1 >> note2;

  auto it1 = db.find(note1);
  auto it2 = db.find(note2);
  if (it1 == db.end() || it2 == db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }

  for (size_t i = 0; i < it1->second->links.size(); ++i)
  {
    auto sp = it1->second->links[i].lock();

    if (sp == it2->second)
    {
      throw std::logic_error("duplicate");
    }
  }

  it1->second->links.push_back(it2->second);
}

void lachugin::removeNote(std::istream &in, std::ostream &, notesMap &db)
{
  std::string note;
  in >> note;

  auto it = db.find(note);
  if (it == db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }

  db.erase(note);
}

void lachugin::removeLink(std::istream &in, std::ostream &, notesMap &db)
{
  std::string note1, note2;
  in >> note1 >> note2;

  auto it1 = db.find(note1);
  auto it2 = db.find(note2);
  if (it1 == db.end() || it2 == db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }

  for (size_t i = 0; i < it1->second->links.size(); ++i)
  {
    auto sp = it1->second->links[i].lock();

    if (sp && sp == it2->second)
    {
      it1->second->links.erase(it1->second->links.begin() + i);
      return;
    }
  }
}

void lachugin::printLinks(std::istream &in, std::ostream &out, notesMap &db)
{
  std::string note;
  in >> note;

  bool first = true;

  auto it = db.find(note);
  if (it == db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }

  for (size_t i = 0; i < it->second->links.size(); ++i)
  {
    auto sp = it->second->links[i].lock();
    if (sp)
    {
      if (first)
      {
        out << sp->name_;
        first = false;
      }
      else
      {
        out << "\n" << sp->name_;
      }
    }
  }
}

void lachugin::allRemovedNotes(std::istream &in, std::ostream &out, notesMap &db)
{
  std::string note;
  in >> note;

  auto it = db.find(note);
  if (it == db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }

  size_t count = 0;

  for (size_t i = 0; i < it->second->links.size(); ++i)
  {
    auto sp = it->second->links[i].lock();
    if (!sp)
    {
      ++count;
    }
  }

  out << count;
}

void lachugin::linksRemover(std::istream &in, std::ostream &, notesMap &db)
{
  std::string note;
  in >> note;

  auto it = db.find(note);
  if (it == db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }

  size_t i = 0;

  while (i < it->second->links.size())
  {
    auto sp = it->second->links[i].lock();

    if (!sp)
    {
      it->second->links.erase(it->second->links.begin() + i);
    }
    else
    {
      ++i;
    }
  }
}

bool lachugin::dfsLoop(const std::shared_ptr< Note >& start, const std::shared_ptr< Note >& cur,
    size_t d, size_t max, std::vector< std::shared_ptr< Note > >& path)
{
  if (d > max + 1)
  {
    return false;
  }

  for (size_t i = 0; i < cur->links.size(); ++i)
  {
    auto next = cur->links[i].lock();
    if (!next)
    {
      continue;
    }
    if (next == start)
    {
      path.push_back(start);
      return true;
    }

    bool used = false;
    for (size_t j = 0; j < path.size(); ++j)
    {
      if (path[j] == next)
      {
        used = true;
        break;
      }
    }

    if (used)
    {
      continue;
    }
    path.push_back(next);
    if (dfsLoop(start, next, d + 1, max, path))
    {
      return true;
    }

    path.pop_back();
  }

  return false;
}
