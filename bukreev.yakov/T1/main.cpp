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

  void noteCommand(std::istream& in);
  void lineCommand(std::istream& in);
  void showCommand(std::istream& in, std::ostream& out);
  void dropCommand(std::istream& in);
  void linkCommand(std::istream& in);
  void haltCommand(std::istream& in);
  void mindCommand(std::istream& in, std::ostream& out);
  void expiredCommand(std::istream& in, std::ostream& out);
  void refreshCommand(std::istream& in);
}

int main()
{
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      bukreev::executeCommand(cmd, std::cin, std::cout);
    }
    catch(const std::logic_error& e)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

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
  }
  else if (cmd == "expired")
  {
    bukreev::expiredCommand(in, out);
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

  notesMap[name] = std::make_shared< Note >(name);
}

void bukreev::lineCommand(std::istream& in)
{
  std::string name, line;
  if (!(in >> name >> std::quoted(line)))
  {
    throw std::logic_error("Invalid input");
  }

  if (!notesMap.count(name))
  {
    throw std::logic_error("Note does not exist");
  }

  notesMap[name]->appendLine(line);
}

void bukreev::showCommand(std::istream& in, std::ostream& out)
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

  notesMap[name]->show(out);
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

  if (!notesMap.count(from) || !notesMap.count(to))
  {
    throw std::logic_error("Note does not exist");
  }

  notesMap[from]->addLink(notesMap[to]);
}

void bukreev::haltCommand(std::istream& in)
{
  std::string from, to;

  if (!(in >> from >> to))
  {
    throw std::logic_error("Invalid input");
  }

  if (!notesMap.count(from) || !notesMap.count(to))
  {
    throw std::logic_error("Note does not exist");
  }

  notesMap[from]->removeLink(to);
}

void bukreev::mindCommand(std::istream& in, std::ostream& out)
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

  notesMap[name]->showLinks(out);
}

void bukreev::expiredCommand(std::istream& in, std::ostream& out)
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

  out << notesMap[name]->countExpired() << '\n';
}

void bukreev::refreshCommand(std::istream& in)
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

  notesMap[name]->removeExpired();
}
