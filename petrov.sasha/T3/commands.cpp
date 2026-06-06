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
}
