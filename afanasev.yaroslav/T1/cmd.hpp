#ifndef CMD_HPP
#define CMD_HPP

#include <iosfwd>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "note.hpp"

namespace afanasev
{
  using note_t = std::unordered_map< std::string, std::shared_ptr< Note > >;
  using func_t = void (*)(std::istream &, std::ostream &, note_t &);

  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();
}

#endif
