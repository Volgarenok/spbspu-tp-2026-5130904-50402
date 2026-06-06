#include "shapes.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <numeric>
#include <utility>

namespace petrov
{
  using namespace std::placeholders;


  bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }
  
  IOguard::IOguard(std::istream& stream):
    stream_(stream),
    flags_(stream.flags())
  {}

  IOguard::~IOguard() {
    stream_.flags(flags_);
  }

  std::istream& operator>>(std::istream& in, DelimIO&& delim) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = 0;
    in >> c;
    if (in && c != delim.expected) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Point& point) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    Point pt{0, 0};
    in >> DelimIO{'('} >> pt.x >> DelimIO{';'} >> pt.y >> DelimIO{')'};
    if (in) {
      point = pt;
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Polygon& polygon) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    IOguard guard(in);
    size_t count = 0;
    if (!(in >> count) || count < 3) {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::vector< Point > points;
    points.reserve(count);
    std::copy_n(std::istream_iterator< Point >(in), count, std::back_inserter(points));
    if (in) {
      polygon.points = std::move(points);
    }
    return in;
  }

  bool compareByX(const Point& a, const Point& b) {
    return a.x < b.x;
  }

  bool compareByY(const Point& a, const Point& b) {
    return a.y < b.y;
  }

  bool comparePoints(const Point& a, const Point& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
  }

  Point translatePoint(const Point& p, int dx, int dy) {
    return {p.x - dx, p.y - dy};
  }

  Point translatePoint(const Point& p, int dx, int dy) {
    return {p.y, p.x};
  }

  Point swapPointXY(const Point& p) {
    return {p.y, p.x};
  }

  double crossTerm(const std::vector< Point >& pts, size_t i, size_t n) {
    size_t j = (i + 1) % n;
    return static_cast< double >(pts[i].x) * static_cast< double >(pts[j].y)
    - static_cast< double >(pts[j].x) * static_cast< double >(pts[i].y);
  }

  double calculateArea(const Polygon& poly) {
    const std::vector< Point >& pts = poly.points;
    size_t n = pts.size();
    if (n < 3) {
      return 0.0;
    }
    std::vector< size_t > indices(n);
    std::iota(indices.begin(), indices.end(), 0);
    std::vector< double > terms(n);
    std::transform(indices.begin(), indices.end(), terms.begin(), std::bind(crossTerm, std::cref(pts), _1, n));
    double sum = std::accumulate(terms.begin(), terms.end(), 0.0);
    return std::abs(sum) / 2.0;
  }

  Polygon swapCoordinates(const Polygon& poly) {
    Polygon res;
    res.points.reserve(poly.points.size());
    std::transform(poly.points.begin(), poly.points.end(), std::back_inserter(res.points), swapPointXY);
    return res;
  }

  std::vector< Point > normalize(const Polygon& poly) {
    std::vector< Point > pts = poly.points;
    if (pts.empty()) {
      return pts;
    }
    int minX = std::min_element(pts.begin(), pts.end(), compareByX)->x;
    int minY = std::min_element(pts.begin(), pts.end(), compareByY)->y;
    std::transform(pts.begin(), pts.end(), pts.begin(), std::bind(translatePoint, _1, minX, minY));
    std::sort(pts.begin(), pts.end(), comparePoints);
    return pts;
  }

  bool petrov::isPermutationOf(const Polygon& a, const Polygon& b) {
    if (a.points.size() != b.points.size()) {
      return false;
    }
    return normalize(a) == normalize(b);
  }
}
