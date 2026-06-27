#include "shapes.hpp"
#include <algorithm>
#include <delimiter.hpp>
#include <functional>
#include <ioguard.hpp>
#include <istream>
#include <iterator>
#include <numeric>
#include <vector>

namespace
{
  using vecp_t = const std::vector< karpovich::Point >;
  double crossTerm(vecp_t &pts, size_t i, size_t n)
  {
    size_t j = (i + 1) % n;
    return pts[i].x * pts[j].y - pts[j].x * pts[i].y;
  }
  bool checkRightAngle(vecp_t &pts, size_t i, size_t n)
  {
    size_t prev = (i + n - 1) % n;
    size_t next = (i + 1) % n;
    int dx1 = pts[i].x - pts[prev].x;
    int dy1 = pts[i].y - pts[prev].y;
    int dx2 = pts[next].x - pts[i].x;
    int dy2 = pts[next].y - pts[i].y;
    return dx1 * dx2 + dy1 * dy2 == 0;
  }
  karpovich::Point translatePoint(karpovich::Point origin, karpovich::Point p)
  {
    return karpovich::Point{p.x - origin.x, p.y - origin.y};
  }

  bool checkShift(vecp_t &n1, vecp_t &n2, size_t shift, size_t n)
  {
    std::vector< karpovich::Point > rotated(n);
    std::rotate_copy(n2.begin(), n2.begin() + shift, n2.end(), rotated.begin());
    return std::equal(n1.begin(), n1.end(), rotated.begin());
  }
}

bool karpovich::operator==(const Point &lhs, const Point &rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

std::istream &karpovich::operator>>(std::istream &in, Point &point)
{
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
std::istream &karpovich::operator>>(std::istream &in, Polygon &polygon)
{
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

double karpovich::calculateArea(const Polygon &polygon)
{
  size_t n = polygon.points.size();
  std::vector< size_t > idxs(n);
  std::iota(idxs.begin(), idxs.end(), 0);
  std::vector< double > terms(n);
  auto func = std::bind(crossTerm, std::cref(polygon.points), std::placeholders::_1, n);
  std::transform(idxs.begin(), idxs.end(), terms.begin(), func);
  double sum = std::accumulate(terms.begin(), terms.end(), 0.0, std::plus< double >());
  return std::abs(sum) / 2.0;
}

bool karpovich::hasRightAngle(const Polygon &polygon)
{
  size_t n = polygon.points.size();
  if (n < 3) {
    return false;
  }
  std::vector< size_t > idxs(n);
  std::iota(idxs.begin(), idxs.end(), 0);
  auto func = std::bind(checkRightAngle, std::cref(polygon.points), std::placeholders::_1, n);
  return std::any_of(idxs.begin(), idxs.end(), func);
}

bool karpovich::isSame(const Polygon &p1, const Polygon &p2)
{
  if (p1.points.size() != p2.points.size()) {
    return false;
  }
  if (p1.points.empty()) {
    return true;
  }
  size_t n = p1.points.size();
  std::vector< Point > n1(n), n2(n);

  const Point &orig1 = p1.points.front();
  const Point &orig2 = p2.points.front();

  auto func1 = std::bind(translatePoint, orig1, std::placeholders::_1);
  auto func2 = std::bind(translatePoint, orig2, std::placeholders::_1);
  std::transform(p1.points.begin(), p1.points.end(), n1.begin(), func1);
  std::transform(p2.points.begin(), p2.points.end(), n2.begin(), func2);

  std::vector< size_t > shifts(n);
  std::iota(shifts.begin(), shifts.end(), 0);

  auto func3 = std::bind(checkShift, std::cref(n1), std::cref(n2), std::placeholders::_1, n);
  return std::any_of(shifts.begin(), shifts.end(), func3);
}
