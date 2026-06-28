#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <cstddef>
#include <istream>
#include <vector>

namespace petrov
{
  struct Point
  {
    int x;
    int y;
  };

  bool operator==(const Point &lhs, const Point &rhs);

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream &operator>>(std::istream &in, Point &point);
  std::istream &operator>>(std::istream &in, Polygon &polygon);

  bool compareByX(const Point &a, const Point &b);
  bool compareByY(const Point &a, const Point &b);
  bool comparePoints(const Point &a, const Point &b);
  Point translatePoint(const Point &p, int dx, int dy);
  Point swapPointXY(const Point &p);
  double crossTerm(const std::vector< Point > &pts, std::size_t i, std::size_t n);

  double calculateArea(const Polygon &poly);
  Polygon swapCoordinates(const Polygon &poly);
  std::vector< Point > normalize(const Polygon &poly);
  bool isPermutationOf(const Polygon &a, const Polygon &b);
}

#endif
