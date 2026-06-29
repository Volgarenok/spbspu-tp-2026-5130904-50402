#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <iosfwd>
#include <vector>

namespace matveev
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

  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
}

#endif
