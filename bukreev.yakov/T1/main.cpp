#include <iostream>
#include <unordered_map>
#include <memory>
#include <limits>
#include <iomanip>
#include "note.hpp"

namespace bukreev
{
  std::unordered_map< std::string, std::shared_ptr< Note > > notesMap;

  void executeCommand(std::string cmd, std::istream& in, std::ostream& out);

  void noteCommand(std::istream& in, std::ostream& out);
  void lineCommand(std::istream& in, std::ostream& out);
  void showCommand(std::istream& in, std::ostream& out);
  void dropCommand(std::istream& in, std::ostream& out);
  void linkCommand(std::istream& in, std::ostream& out);
  void haltCommand(std::istream& in, std::ostream& out);
  void mindCommand(std::istream& in, std::ostream& out);
  void expiredCommand(std::istream& in, std::ostream& out);
  void refreshCommand(std::istream& in, std::ostream& out);
  void invalidCommand(std::istream& in, std::ostream& out);
}

int main()
{
  std::string cmd;
  while (std::cin >> cmd)
  {
    bukreev::executeCommand(cmd, std::cin, std::cout);
  }
}

void bukreev::executeCommand(std::string cmd, std::istream& in, std::ostream& out)
{
  if (cmd == "note")
  {
    bukreev::noteCommand(in, out);
  }
  else if (cmd == "line")
  {
    bukreev::lineCommand(in, out);
  }
  else if (cmd == "show")
  {
    bukreev::showCommand(in, out);
  }
  else if (cmd == "drop")
  {
    bukreev::dropCommand(in, out);
  }
  else if (cmd == "link")
  {
    bukreev::linkCommand(in, out);
  }
  else if (cmd == "halt")
  {
    bukreev::haltCommand(in, out);
  }
  else if (cmd == "mind")
  {
    bukreev::mindCommand(in, out);
  }
  else if (cmd == "expired")
  {
    bukreev::expiredCommand(in, out);
  }
  else if (cmd == "refresh")
  {
    bukreev::refreshCommand(in, out);
  }
  else
  {
    bukreev::invalidCommand(in, out);
  }
}

void bukreev::noteCommand(std::istream& in, std::ostream& out)
{
  std::string name;
  if (!(in >> name))
  {
    invalidCommand(in, out);
    return;
  }

  notesMap[name] = std::make_shared< Note >(name);
}

void bukreev::lineCommand(std::istream& in, std::ostream& out)
{
  std::string name, line;
  if (!(in >> name >> std::quoted(line)))
  {
    invalidCommand(in, out);
    return;
  }

  if (!notesMap.count(name))
  {
    invalidCommand(in, out);
    return;
  }

  notesMap[name]->appendLine(line);
}

void bukreev::showCommand(std::istream& in, std::ostream& out)
{
  std::string name;
  if (!(in >> name))
  {
    invalidCommand(in, out);
    return;
  }

  if (!notesMap.count(name))
  {
    invalidCommand(in, out);
    return;
  }

  notesMap[name]->show(out);
}

void bukreev::dropCommand(std::istream& in, std::ostream& out)
{
  std::string name;
  if (!(in >> name))
  {
    invalidCommand(in, out);
    return;
  }

  if (!notesMap.count(name))
  {
    invalidCommand(in, out);
    return;
  }

  notesMap.erase(name);
}

void bukreev::linkCommand(std::istream& in, std::ostream& out)
{
  std::string from, to;

  if (!(in >> from >> to))
  {
    invalidCommand(in, out);
    return;
  }

  if (!notesMap.count(from) || !notesMap.count(to))
  {
    invalidCommand(in, out);
    return;
  }

  try
  {
    notesMap[from]->addLink(notesMap[to]);
  }
  catch(const std::logic_error& e)
  {
    invalidCommand(in, out);
    return;
  }
}

void bukreev::haltCommand(std::istream& in, std::ostream& out)
{
  std::string from, to;

  if (!(in >> from >> to))
  {
    invalidCommand(in, out);
    return;
  }

  if (!notesMap.count(from) || !notesMap.count(to))
  {
    invalidCommand(in, out);
    return;
  }

  notesMap[from]->removeLink(to);
}

void bukreev::mindCommand(std::istream& in, std::ostream& out)
{
  std::string name;

  if (!(in >> name))
  {
    invalidCommand(in, out);
    return;
  }

  if (!notesMap.count(name))
  {
    invalidCommand(in, out);
    return;
  }

  notesMap[name]->showLinks(out);
}

void bukreev::expiredCommand(std::istream& in, std::ostream& out)
{
  std::string name;

  if (!(in >> name))
  {
    invalidCommand(in, out);
    return;
  }

  if (!notesMap.count(name))
  {
    invalidCommand(in, out);
    return;
  }

  out << notesMap[name]->countExpired() << '\n';
}

void bukreev::refreshCommand(std::istream& in, std::ostream& out)
{
  std::string name;

  if (!(in >> name))
  {
    invalidCommand(in, out);
    return;
  }

  if (!notesMap.count(name))
  {
    invalidCommand(in, out);
    return;
  }

  notesMap[name]->removeExpired();
}

void bukreev::invalidCommand(std::istream& in, std::ostream& out)
{
  out << "<INVALID COMMAND>\n";
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}
