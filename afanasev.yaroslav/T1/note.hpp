#ifndef NOTE_HPP
#define NOTE_HPP

#include <iosfwd>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace afanasev
{
  class Note
  {
  public:
    std::vector< std::string > text;
    std::unordered_map< std::string, std::weak_ptr< Note > > ptr;
  };
}

#endif
