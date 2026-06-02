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

    private:
      static Triangle helpArea(const std::vector< Point >& points, size_t& index);
  };

  void area(std::istream& is, std::vector< Polygon > plgs);
  void areaEven(const std::vector< Polygon >& plgs);
  void areaOdd(const std::vector< Polygon >& plgs);
  void areaMean(const std::vector< Polygon >& plgs);
  void areaVrtxs(std::vector< Polygon > plgs, size_t n);
  void max(std::istream& is, std::vector< Polygon > plgs);
  void min(std::istream& is, std::vector< Polygon > plgs);
  void count(std::istream& is, std::vector< Polygon > plgs);
  void maxseq(std::istream& is, std::vector< Polygon > plgs);
  void intersections(std::istream& is, std::vector< Polygon > plgs);

  static bool isEven(Polygon p);
  static bool isOdd(Polygon p);
}
