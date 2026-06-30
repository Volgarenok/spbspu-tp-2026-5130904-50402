#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <istream>
#include <ostream>
#include <vector>
#include "figures.hpp"

namespace novikov
{
  using plg_t = std::vector< novikov::Polygon >;

  void area(std::istream&, std::ostream&, novikov::plg_t&);
  void max(std::istream&, std::ostream&, novikov::plg_t&);
  void min(std::istream&, std::ostream&, novikov::plg_t&);
  void count(std::istream&, std::ostream&, novikov::plg_t&);
  void intersections(std::istream&, std::ostream&, novikov::plg_t&);
  void same(std::istream&, std::ostream&, novikov::plg_t&);
}

#endif
