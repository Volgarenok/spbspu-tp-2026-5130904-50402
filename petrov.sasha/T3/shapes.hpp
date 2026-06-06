#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <istream>
#include <vector>
namespace petrov
{
  struct Point
  {
    int x, y;
  };
  bool operator==(const Point& lhs, const Point& rhs);

  struct Polygon
  {
    std::vector< Point > points;
  };
}
