#include <iostream>
#include <unordered_map>
#include <memory>
#include <limits>
#include <iomanip>
#include "note.hpp"

namespace bukreev
{
  std::unordered_map< std::string, std::shared_ptr< Note > > notesMap;

  void noteCommand(std::istream& in, std::ostream& out);
  void lineCommand(std::istream& in, std::ostream& out);
}

int main()
{
  std::string cmd;
  while (std::cin >> cmd)
  {
    if (cmd == "note")
    {
      bukreev::noteCommand(std::cin, std::cout);
    }
    else if (cmd == "line")
    {
      bukreev::lineCommand(std::cin, std::cout);
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

void bukreev::noteCommand(std::istream& in, std::ostream& out)
{
  std::string name;
  if (!(in >> name))
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  notesMap[name] = std::make_shared< Note >();
}

void bukreev::lineCommand(std::istream& in, std::ostream& out)
{
  std::string name, line;
  if (!(in >> name >> std::quoted(line)))
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  if (!notesMap.count(name))
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  notesMap[name]->appendLine(line);
}
