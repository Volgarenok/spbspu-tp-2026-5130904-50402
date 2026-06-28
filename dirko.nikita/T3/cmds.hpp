#ifndef CMDS_HPP
#define CMDS_HPP
#include <iostream>
#include <vector>
#include "polygon.hpp"
namespace dirko
{
  void area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void rects(std::istream &, std::ostream &out, const std::vector< Polygon > &polygons);
  void inframe(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
}

#endif
