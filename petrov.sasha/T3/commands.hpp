#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <vector>
#include "shapes.hpp"

namespace petrov
{
  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void perms(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void inframe(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
}

#endif
