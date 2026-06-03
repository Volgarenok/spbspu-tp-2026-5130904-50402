#ifndef AREA_HPP
#define AREA_HPP
#include "Figures.hpp"

namespace lavrentev
{
  void area(std::istream& is, const std::vector< Polygon > plgs);
  void areaEven(std::istream&, const std::vector< Polygon >& plgs);
  void areaOdd(std::istream&, const std::vector< Polygon >& plgs);
  void areaMean(std::istream&, const std::vector< Polygon >& plgs);
  void areaVrtxs(const std::vector< Polygon >& plgs, size_t n);
}

#endif
