#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <istream>
#include <ostream>
#include <vector>
#include "figures.hpp"

namespace shirokov
{
  using plg_t = std::vector< Polygon >;

  void area(std::istream&, std::ostream&, plg_t&);
  void max(std::istream&, std::ostream&, plg_t&);
  void min(std::istream&, std::ostream&, plg_t&);
  void count(std::istream&, std::ostream&, plg_t&);
  void rects(std::istream&, std::ostream&, plg_t&);
  void maxseq(std::istream&, std::ostream&, plg_t&);
}

#endif
