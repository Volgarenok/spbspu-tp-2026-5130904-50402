#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <iosfwd>
#include <vector>

namespace chernikov {
  struct Point
  {
    int x, y;
  };

  bool operator<(const Point &a, const Point &b);
  bool operator==(const Point &a, const Point &b);

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream &operator>>(std::istream &in, Point &point);
  std::istream &operator>>(std::istream &in, Polygon &polygon);
  double calcArea(const Polygon &polygon);
  bool isRect(const Polygon &polygon);
  bool hasRightAngle(const Polygon &polygon);
  bool isPermutationOf(const Polygon &a, const Polygon &b);
}
#endif
