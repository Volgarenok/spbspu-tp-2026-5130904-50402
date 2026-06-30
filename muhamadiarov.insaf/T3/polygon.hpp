#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <delimiterio.hpp>

namespace muhamadiarov
{
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  bool operator==(const Point& lhs, const Point& rhs);
  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
}

#endif
