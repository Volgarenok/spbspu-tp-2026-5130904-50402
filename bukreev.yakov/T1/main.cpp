#include <iostream>
#include <unordered_map>
#include <memory>
#include <limits>
#include "note.hpp"

namespace bukreev
{
  std::unordered_map< std::string, std::shared_ptr< Note > > notesMap;

  void noteCommand(std::istream& in, std::ostream& out);
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
