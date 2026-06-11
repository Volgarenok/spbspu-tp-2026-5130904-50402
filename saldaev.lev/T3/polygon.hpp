#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <iostream>
#include <vector>

namespace saldaev
{
  struct Point
  {
    int x, y;
    bool operator==(const Point &other) const;
  };

  struct Polygon
  {
    std::vector< Point > points;
    bool operator==(const Polygon &other) const;
  };

  std::istream &operator>>(std::istream &in, Point &point);
  std::istream &operator>>(std::istream &in, Polygon &polygon);
  double calcArea(const Polygon &polygon);
  bool isRect(const Polygon &polygon);
}

#endif
