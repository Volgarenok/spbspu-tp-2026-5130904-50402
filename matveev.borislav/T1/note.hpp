#ifndef MATVEEV_NOTE_HPP
#define MATVEEV_NOTE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace matveev
{
  struct Note
  {
    std::string name;
    std::vector< std::string > lines;
    std::vector< std::pair< std::string, std::weak_ptr< Note > > > links;
  };

  using db_t = std::unordered_map< std::string, std::shared_ptr< Note > >;
}

#endif
