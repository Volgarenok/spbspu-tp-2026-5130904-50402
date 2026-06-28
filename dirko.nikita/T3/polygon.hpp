#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>
namespace dirko
{
  struct Point
  {
    int x, y;
  };
  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream &operator>>(std::istream &in, Point &point);
  std::istream &operator>>(std::istream &in, Polygon &polygon);
  double calcArea(const Polygon &polygon);
  bool isRect(const Polygon &polygon);
}

#endif
