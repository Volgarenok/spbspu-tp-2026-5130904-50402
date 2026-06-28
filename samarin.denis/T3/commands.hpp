#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <vector>
#include "Polygon.hpp"

namespace samarin {
  void processCommands(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out);
}

#endif
