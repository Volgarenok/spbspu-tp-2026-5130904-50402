#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <common.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <cctype>

namespace afanasev
{
  struct Point
  {
    int x, y;
  };

  bool operator==(const Point & lhs, const Point & rhs);

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream & operator>>(std::istream & in, Point & point);

  std::istream & operator>>(std::istream & in, Polygon & polygon);

  double makeArea(const Polygon & polygon);
  Polygon swapCoordinates(const Polygon & p);
  std::vector< Point > normalize(const Polygon & p);
  bool isPermutationOf(const Polygon & a, const Polygon & b);
  bool isRectangle(const Polygon & poly);
}

#endif
