#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <iostream>
#include <vector>
namespace karpovich
{
  struct Point
  {
    int x, y;
  };
  bool operator==(const Point &lhs, const Point &rhs);
  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream &operator>>(std::istream &in, Point &point);
  std::istream &operator>>(std::istream &in, Polygon &polygon);
  double calculateArea(const Polygon &polygon);
  bool hasRightAngle(const Polygon &polygon);
  bool isSame(const Polygon &p1, const Polygon &p2);
}

#endif
