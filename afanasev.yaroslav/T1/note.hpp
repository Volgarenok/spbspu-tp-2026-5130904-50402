#ifndef NOTE_HPP
#define NOTE_HPP

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
    std::vector< std::string > text_;
    std::vector< std::pair< std::string, std::weak_ptr< Note > > > ptr_;
  };
}

#endif
