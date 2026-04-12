#ifndef NOTE_HPP
#define NOTE_HPP
#include <vector>
#include <memory>
namespace lachugin
{
  struct Note
  {
    Note(std::string& name): name(name) {}
    std::string name;
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > links;
  };

}
#endif
