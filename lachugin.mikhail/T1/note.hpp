#ifndef NOTE_HPP
#define NOTE_HPP
#include <vector>
#include <iostream>

namespace lachugin
{
  struct Note
  {
    Note(const std::string& name): name(name) {}
    std::string name;
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > links;
  };

}
#endif
