#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <iosfwd>
#include <vector>

namespace chernikov
{

  struct Point
  {
    int x;
    int y;
  };

  bool operator==(const Point &a, const Point &b);
  bool operator<(const Point &a, const Point &b);
  std::istream &operator>>(std::istream &in, Point &point);
  std::ostream &operator<<(std::ostream &out, const Point &point);

  struct Polygon
  {
    std::vector<Point> points;
  };

  bool operator==(const Polygon &a, const Polygon &b);
  bool operator<(const Polygon &a, const Polygon &b);
  std::istream &operator>>(std::istream &in, Polygon &polygon);
  std::ostream &operator<<(std::ostream &out, const Polygon &polygon);

  double computeArea(const Polygon &polygon);
  bool isRectangle(const Polygon &polygon);
  bool hasRightAngle(const Polygon &polygon);
  bool isPermutationOf(const Polygon &a, const Polygon &b);

}

#endif
