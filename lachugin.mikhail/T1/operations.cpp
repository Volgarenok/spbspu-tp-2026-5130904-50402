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

  bool printed = false;

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
      out << sp->name_ << "\n";
      printed = true;
    }
  }
  if (!printed)
  {
    out << "\n";
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

  out << count << "\n";
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
