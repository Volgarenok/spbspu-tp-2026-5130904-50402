#include "operations.hpp"
#include "note.hpp"
#include <vector>
#include <iomanip>
#include <string>
#include <unordered_map>

void lachugin::addNote(std::istream& in, std::ostream &, notesMap &db)
{
  std::string note;
  in >> note;
  auto it = db.find(note);
  if (it != db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }
  auto new_note = std::make_shared<Note>(note);
  db[note] = new_note;
}

void lachugin::addLine(std::istream &in, std::ostream &, notesMap &db)
{
  std::string note;
  in >> note;

  auto it = db.find(note);
  if (it == db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }

  std::string str;
  in >> std::quoted(str);
  db[note]->lines.push_back(str);

}

void lachugin::showNote(std::istream &in, std::ostream &out, notesMap &db)
{
  std::string note;
  in >> note;

  auto it = db.find(note);
  if (it == db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }
  if (it->second->lines.empty())
  {
    return;
  }
  for (size_t i = 0; i < it->second->lines.size(); ++i)
  {
    out << it->second->lines[i] << "\n";
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

    if (sp && sp == it2->second)
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

  bool found = false;

  for (size_t i = 0; i < it1->second->links.size(); ++i)
  {
    auto sp = it1->second->links[i].lock();

    if (sp && sp == it2->second)
    {
      it1->second->links.erase(it1->second->links.begin() + i);
      found = true;
      break;
    }
  }

  if (!found)
  {
    throw std::logic_error("no link");
  }
}

void lachugin::printLinks(std::istream &in, std::ostream &out, notesMap &db)
{
  std::string note;
  in >> note;

  auto it = db.find(note);
  if (it == db.end())
  {
    throw std::logic_error ("Err: note not exist");
  }

  bool first = true;

  for (size_t i = 0; i < it->second->links.size(); ++i)
  {
    auto sp = it->second->links[i].lock();

    if (sp)
    {
      if (!first)
      {
        out << "\n";
      }

      out << sp->name;
      first = false;
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
/*
refresh:
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

expired:
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

halt:
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

bool found = false;

for (size_t i = 0; i < it1->second->links.size(); ++i)
{
  auto sp = it1->second->links[i].lock();

  if (sp && sp == it2->second)
  {
    it1->second->links.erase(it1->second->links.begin() + i);
    found = true;
    break;
  }
}

if (!found)
{
  throw std::logic_error("no link");
}
catch (...)
{
  std::cout << "<INVALID COMMAND>\n";
  auto toignore = std::numeric_limits<std::streamsize>::max();
  std::cin.ignore(toignore, '\n');
}

bool first = true;

for (size_t i = 0; i < it->second->links.size(); ++i)
{
  auto sp = it->second->links[i].lock();

  if (sp)
  {
    if (!first)
    {
      out << "\n";
    }

    out << sp->name;
    first = false;
  }
}
 */
