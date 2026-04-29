#ifndef OP_HPP
#define OP_HPP
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "note.hpp"

namespace novikov
{
  using mapNotes = std::unordered_map< std::string, std::shared_ptr< Note > >;
  using cmd_t = void (*)(std::istream &, std::ostream &, mapNotes &);

}

#endif