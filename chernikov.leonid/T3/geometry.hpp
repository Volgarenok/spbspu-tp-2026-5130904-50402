#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <iosfwd>
#include <vector>

namespace chernikov {
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream &operator>>(std::istream &in, Point &point);
  std::istream &operator>>(std::istream &in, Polygon &polygon);
  std::ostream &operator<<(std::ostream &out, const Point &point);
  std::ostream &operator<<(std::ostream &out, const Polygon &polygon);

  double calcArea(const Polygon &polygon);
  bool isRect(const Polygon &polygon);
  bool isPermutationOf(const Polygon &a, const Polygon &b);
  bool hasRightAngle(const Polygon &polygon);
  bool operator==(const Point &a, const Point &b);
  bool operator<(const Point &a, const Point &b);
  bool operator==(const Polygon &a, const Polygon &b);
}
#endif
