#include "geometry.hpp"
#include <algorithm>
#include <delim.hpp>
#include <functional>
#include <ioguard.hpp>
#include <iterator>
#include <numeric>
#include <set>

namespace {
  double crossTerm(const std::vector< chernikov::Point > &pts, size_t i, size_t n)
  {
    size_t j = (i + 1) % n;
    return pts[i].x * pts[j].y - pts[j].x * pts[i].y;
  }

  bool checkRightAngle(const std::vector< chernikov::Point > &pts, size_t i, size_t n)
  {
    size_t prev = (i + n - 1) % n;
    size_t next = (i + 1) % n;
    int dx1 = pts[i].x - pts[prev].x;
    int dy1 = pts[i].y - pts[prev].y;
    int dx2 = pts[next].x - pts[i].x;
    int dy2 = pts[next].y - pts[i].y;
    return dx1 * dx2 + dy1 * dy2 == 0;
  }
}

std::istream &chernikov::operator>>(std::istream &in, Point &point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Point pt{0, 0};
  in >> DelimIO{'('} >> pt.x >> DelimIO{';'} >> pt.y >> DelimIO{')'};
  if (in)
  {
    point = pt;
  }
  return in;
}

std::istream &chernikov::operator>>(std::istream &in, Polygon &polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  IOguard guard(in);
  size_t count = 0;
  if (!(in >> count) || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > points;
  points.reserve(count);
  std::copy_n(std::istream_iterator< Point >(in), count, std::back_inserter(points));
  if (in)
  {
    polygon.points = std::move(points);
  }
  return in;
}

double chernikov::calcArea(const Polygon &polygon)
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

bool chernikov::isRect(const Polygon &polygon)
{
  if (polygon.points.size() != 4)
  {
    return false;
  }
  std::vector< size_t > idxs(4);
  std::iota(idxs.begin(), idxs.end(), 0);
  auto func = std::bind(checkRightAngle, std::cref(polygon.points), std::placeholders::_1, 4);
  return std::all_of(idxs.begin(), idxs.end(), func);
}

bool chernikov::hasRightAngle(const Polygon &polygon)
{
  size_t n = polygon.points.size();
  if (n < 3)
  {
    return false;
  }
  std::vector< size_t > idxs(n);
  std::iota(idxs.begin(), idxs.end(), 0);
  auto func = std::bind(checkRightAngle, std::cref(polygon.points), std::placeholders::_1, n);
  return std::any_of(idxs.begin(), idxs.end(), func);
}

bool chernikov::isPermutationOf(const Polygon &a, const Polygon &b)
{
  if (a.points.size() != b.points.size())
  {
    return false;
  }
  std::multiset< Point > setA(a.points.begin(), a.points.end());
  std::multiset< Point > setB(b.points.begin(), b.points.end());
  return setA == setB;
}
