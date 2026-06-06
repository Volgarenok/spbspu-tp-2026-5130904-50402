#ifndef NOTE_HPP
#define NOTE_HPP
#include <memory>
#include <string>
#include <vector>

namespace pozdnyakov
{

  struct Note
  {
    std::string name;
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > links;

    explicit Note(const std::string &n);
  };

}

#endif
