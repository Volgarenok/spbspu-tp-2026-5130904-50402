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

  void area(std::istream& is, const std::vector< Polygon > plgs);
  void areaEven(std::istream&, const std::vector< Polygon >& plgs);
  void areaOdd(std::istream&, const std::vector< Polygon >& plgs);
  void areaMean(std::istream&, const std::vector< Polygon >& plgs);
  void areaVrtxs(std::istream& is, const std::vector< Polygon >& plgs);
  void max(std::istream& is, const std::vector< Polygon >& plgs);
  void maxArea(std::istream&, const std::vector< Polygon >& plgs);
  void maxVrtxs(std::istream&, const std::vector< Polygon >& plgs); //TODO
  void min(std::istream& is, const std::vector< Polygon >& plgs); //TODO
  void minArea(std::istream&, const std::vector< Polygon >& plgs); //TODO
  void minVrtxs(std::istream&, const std::vector< Polygon >& plgs); //TODO
  void count(std::istream& is, const std::vector< Polygon >& plgs); //TODO
  void maxseq(std::istream& is, const std::vector< Polygon >& plgs); //TODO
  void intersections(std::istream& is, const std::vector< Polygon >& plgs); //TODO

  static bool isEven(Polygon p);
  static bool isOdd(Polygon p);
  static bool isAmount(Polygon p, size_t n);
}
