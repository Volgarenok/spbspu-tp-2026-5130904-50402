#include "commands.hpp"
#include "note.hpp"
#include <unordered_map>
#include <memory>
#include <limits>
#include <iomanip>

std::unordered_map< std::string, std::shared_ptr< bukreev::Note > > notesMap;

void bukreev::executeCommand(std::string cmd, std::istream& in, std::ostream& out)
{
  if (cmd == "note")
  {
    bukreev::noteCommand(in);
  }
  else if (cmd == "line")
  {
    bukreev::lineCommand(in);
  }
  else if (cmd == "show")
  {
    bukreev::showCommand(in, out);
    std::cout << '\n';
  }
  else if (cmd == "drop")
  {
    bukreev::dropCommand(in);
  }
  else if (cmd == "link")
  {
    bukreev::linkCommand(in);
  }
  else if (cmd == "halt")
  {
    bukreev::haltCommand(in);
  }
  else if (cmd == "mind")
  {
    bukreev::mindCommand(in, out);
    std::cout << '\n';
  }
  else if (cmd == "expired")
  {
    bukreev::expiredCommand(in, out);
    std::cout << '\n';
  }
  else if (cmd == "refresh")
  {
    bukreev::refreshCommand(in);
  }
  else
  {
    throw std::logic_error("Unknown command");
  }
}

void bukreev::noteCommand(std::istream& in)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::logic_error("Invalid input");
  }

  if (notesMap.count(name))
  {
    throw std::logic_error("Note already exists");
  }

  notesMap.insert({name, std::make_shared< Note >(name)});
}

void bukreev::lineCommand(std::istream& in)
{
  std::string name, line;
  if (!(in >> name >> std::quoted(line)))
  {
    throw std::logic_error("Invalid input");
  }

  std::shared_ptr< Note > note = notesMap.at(name);
  note->appendLine(line);
}

void bukreev::showCommand(std::istream& in, std::ostream& out)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::logic_error("Invalid input");
  }

  std::shared_ptr< Note > note = notesMap.at(name);
  note->show(out);
}

void bukreev::dropCommand(std::istream& in)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::logic_error("Invalid input");
  }

  if (!notesMap.count(name))
  {
    throw std::logic_error("Note does not exist");
  }

  notesMap.erase(name);
}

void bukreev::linkCommand(std::istream& in)
{
  std::string from, to;

  if (!(in >> from >> to))
  {
    throw std::logic_error("Invalid input");
  }

  std::shared_ptr< Note > f = notesMap.at(from);
  std::shared_ptr< Note > t = notesMap.at(to);
  f->addLink(t);
}

void bukreev::haltCommand(std::istream& in)
{
  std::string from, to;

  if (!(in >> from >> to))
  {
    throw std::logic_error("Invalid input");
  }

  std::shared_ptr< Note > f = notesMap.at(from);
  f->removeLink(to);
}

void bukreev::mindCommand(std::istream& in, std::ostream& out)
{
  std::string name;

  if (!(in >> name))
  {
    throw std::logic_error("Invalid input");
  }

  std::shared_ptr< Note > note = notesMap.at(name);
  note->showLinks(out);
}

void bukreev::expiredCommand(std::istream& in, std::ostream& out)
{
  std::string name;

  if (!(in >> name))
  {
    throw std::logic_error("Invalid input");
  }

  std::shared_ptr< Note > note = notesMap.at(name);
  out << note->countExpired();
}

void bukreev::refreshCommand(std::istream& in)
{
  std::string name;

  if (!(in >> name))
  {
    throw std::logic_error("Invalid input");
  }

  std::shared_ptr< Note > note = notesMap.at(name);
  note->removeExpired();
}
