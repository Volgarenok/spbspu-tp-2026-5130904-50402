#include "commands.hpp"
#include <algorithm>
#include <cctype>
#include <functional>
#include <iomanip>
#include <iterator>
#include <limits>
#include <numeric>
#include <string>
#include "shapes.hpp"

namespace petrov
{
  using namespace std::placeholders;

  bool isEven(const Polygon& p) {
    return p.points.size() % 2 == 0;
  }

  bool isOdd(const Polygon& p) {
    return p.points.size() % 2 != 0;
  }

  bool hasVertexCount(const Polygon& p, size_t n) {
    return p.points.size() == n;
  }

  double getArea(const Polygon& p) {
    return calculateArea(p);
  }

  bool isDigitChar(char c) {
    return std::isdigit(static_cast< unsigned char >(c));
  }

  bool isNumber(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), isDigitChar);
  }

  bool isSpaceChar(char c) {
    return c == ' ';
  }

  bool areaLess(const Polygon& a, const Polygon& b) {
    return calculateArea(a) < calculateArea(b);
  }

  bool vertexLess(const Polygon& a, const Polygon& b) {
    return a.points.size() < b.points.size();
  }

  struct Bounds
  {
    int minX, minY, maxX, maxY;
  };

  Bounds getPolygonBounds(const Polygon& poly) {
    const std::vector< Point >& pts = poly.points;
    int minX = std::min_element(pts.begin(), pts.end(), compareByX)->x;
    int maxX = std::max_element(pts.begin(), pts.end(), compareByX)->x;
    int minY = std::min_element(pts.begin(), pts.end(), compareByY)->y;
    int maxY = std::max_element(pts.begin(), pts.end(), compareByY)->y;
    return {minX, minY, maxX, maxY};
  }

  bool compareByMinX(const Bounds& a, const Bounds& b) {
    return a.minX < b.minX;
  }

  bool compareByMinY(const Bounds& a, const Bounds& b) {
    return a.minY < b.minY;
  }

  bool compareByMaxX(const Bounds& a, const Bounds& b) {
    return a.maxX < b.maxX;
  }

  bool compareByMaxY(const Bounds& a, const Bounds& b) {
    return a.maxY < b.maxY;
  }

  bool isPointInsideFrame(const Point& p, int minX, int minY, int maxX, int maxY) {
    return p.x >= minX && p.x <= maxX && p.y >= minY && p.y <= maxY;
  }

  template< class Pred >
  void printFilteredSum(std::ostream& out, const std::vector< Polygon >& polygons, Pred pred) {
    std::vector< Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), pred);
    std::vector< double > areas;
    areas.reserve(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << sum << '\n';
  }

  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons) {
    std::string param;
    if (!(in >> param)) {
      throw std::invalid_argument("invalid");
    }
    out << std::fixed << std::setprecision(1);

    if (param == "MEAN") {
      if (polygons.empty()) {
        throw std::invalid_argument("invalid");
      }
      std::vector< double > areas;
      areas.reserve(polygons.size());

      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);
      double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
      out << sum / polygons.size() << "\n";
    } else if (param == "EVEN") {
      printFilteredSum(out, polygons, isEven);
    } else if (param == "ODD") {
      printFilteredSum(out, polygons, isOdd);
    } else if (isNumber(param)) {
      size_t n = std::stoul(param);
      if (n < 3) {
        throw std::invalid_argument("invalid");
      }
      printFilteredSum(out, polygons, std::bind(hasVertexCount, _1, n));
    } else {
      throw std::invalid_argument("invalid");
    }
  }
}
