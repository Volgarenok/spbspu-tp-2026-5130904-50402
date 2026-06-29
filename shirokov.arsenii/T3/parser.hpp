#ifndef PARSER_HPP
#define PARSER_HPP
#include <istream>
#include "commands.hpp"

namespace shirokov
{
  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, Polygon&);
  plg_t parse(std::istream&);
}

#endif
