#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <vector>
#include <iostream>
#include <iterator>

namespace muhamadiarov
{
  struct Point
  {
    int x_;
    int y_;
  };

  struct Polygon
  {
    std::vector< Point > points_;
  };

  bool operator==(const Point& lhs, const Point& rhs);
  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
}

#endif
