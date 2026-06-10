#ifndef CMDS_HPP
#define CMDS_HPP
#include <iostream>
#include <vector>
#include "polygon.hpp"

namespace saldaev
{
  void handleArea(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void handleMax(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void handleMin(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void handleCount(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void handleRects(std::istream &, std::ostream &out, const std::vector< Polygon > &polygons);
  void handleMexseq(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
}

#endif