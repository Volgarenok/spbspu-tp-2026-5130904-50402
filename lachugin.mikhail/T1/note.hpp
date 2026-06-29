#ifndef NOTE_HPP
#define NOTE_HPP
#include <vector>
#include <string>
#include <memory>
namespace lachugin
{
  struct Note
  {
    std::string name_;
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > links;
    Note(const std::string& name);
  };

}
#endif
