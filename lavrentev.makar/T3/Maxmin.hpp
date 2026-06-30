#ifndef MAXMIN_HPP
#define MAXMIN_HPP
#include <istream>
#include "Figures.hpp"

namespace lavrentev
{
  void max(std::istream &is, const std::vector< Polygon > &plgs);
  void maxArea(std::istream &, const std::vector< Polygon > &plgs);
  void maxVrtxs(std::istream &, const std::vector< Polygon > &plgs);
  void min(std::istream &is, const std::vector< Polygon > &plgs);
  void minArea(std::istream &, const std::vector< Polygon > &plgs);
  void minVrtxs(std::istream &, const std::vector< Polygon > &plgs);
}

#endif
