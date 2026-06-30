#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "geometry.hpp"
#include <iosfwd>
#include <vector>

namespace chernikov {
  void area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void perms(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void echo(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons);
  void maxseq(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void rmecho(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons);
  void rects(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void rightshapes(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
}
#endif
