#ifndef ACTION_HPP
#define ACTION_HPP
#include <istream>
#include <memmory>
#include <string>
#include <vector>
#include <utility>

namespace muhamadiarov
{
  struct Note
  {
    std::string name_;
    std::vector< std::string > lines_;
    std::vector< std::pair< std::string, std::weak_ptr< Note > > links_;
  };
}

#endif
