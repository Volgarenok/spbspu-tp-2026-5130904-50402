#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>

namespace pozdnyakov
{

  struct Point
  {
    int x = 0;
    int y = 0;
  };

  bool operator==(const Point &lhs, const Point &rhs);
  bool operator!=(const Point &lhs, const Point &rhs);

  struct Polygon
  {
    std::vector< Point > points{};
  };

  bool operator==(const Polygon &lhs, const Polygon &rhs);

  struct DelimiterIO
  {
    char expected = '\0';
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, Point &dest);
  std::istream &operator>>(std::istream &in, Polygon &dest);

  double calculateAreaEven(const std::vector< Polygon > &polygons);
  double calculateAreaOdd(const std::vector< Polygon > &polygons);
  double calculateAreaMean(const std::vector< Polygon > &polygons);
  double calculateAreaNum(const std::vector< Polygon > &polygons, const size_t num);

  double getMinArea(const std::vector< Polygon > &polygons);
  size_t getMinVertexes(const std::vector< Polygon > &polygons);
  double getMaxArea(const std::vector< Polygon > &polygons);
  size_t getMaxVertexes(const std::vector< Polygon > &polygons);

  size_t countEvenVertices(const std::vector< Polygon > &polygons);
  size_t countOddVertices(const std::vector< Polygon > &polygons);
  size_t countNumVertices(const std::vector< Polygon > &polygons, const size_t num);

  size_t countPermutations(const std::vector< Polygon > &polygons, const Polygon &target);
  size_t countIntersections(const std::vector< Polygon > &polygons, const Polygon &target);

}

#endif
