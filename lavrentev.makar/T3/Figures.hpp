#ifndef FIGURES_HPP
#define FIGURES_HPP
#include <cstddef>
#include <vector>

namespace lavrentev
{
  struct Point
  {
    int x, y;
  };

  struct Triangle
  {
    static const size_t size = 3;
    std::vector< Point > points = std::vector< Point >(size);

    Triangle(Point a, Point b, Point c):
      points{a, b, c}
    {}

    const float getArea() const;
  };

  struct Polygon
  {
    std::vector< Point > points;
    const float getArea() const;
    const size_t getSize() const;

    private:
      static Triangle helpArea(const std::vector< Point >& points, size_t& index);
  };

  void maxseq(std::istream& is, const std::vector< Polygon >& plgs); //TODO
  void intersections(std::istream& is, const std::vector< Polygon >& plgs); //TODO

  bool isEven(Polygon p);
  bool isOdd(Polygon p);
  bool isAmount(Polygon p, size_t n);
}

#endif
