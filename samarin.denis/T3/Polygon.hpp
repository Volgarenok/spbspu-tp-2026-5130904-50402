#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iosfwd>
#include <vector>

namespace samarin {
  struct Point {
    int x;
    int y;
  };

  struct Polygon {
    std::vector< Point > points;
  };

  std::istream &operator>>(std::istream &in, Point &dest);
  std::istream &operator>>(std::istream &in, Polygon &dest);

  double getArea(const Polygon &polygon);
  bool hasRightAngle(const Polygon &polygon);
  bool intersects(const Polygon &lhs, const Polygon &rhs);
}

#endif
