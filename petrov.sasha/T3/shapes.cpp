#include "shapes.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <istream>
#include <iterator>
#include <numeric>
#include <utility>

#include "delim_io.hpp"
#include "ioguard.hpp"

bool petrov::operator==(const petrov::Point &lhs, const petrov::Point &rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

std::istream &petrov::operator>>(std::istream &in, petrov::Point &point)
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

std::istream &petrov::operator>>(std::istream &in, petrov::Polygon &polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  IOGuard guard(in);
  std::size_t count = 0;
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

bool petrov::compareByX(const petrov::Point &a, const petrov::Point &b)
{
  return a.x < b.x;
}

bool petrov::compareByY(const petrov::Point &a, const petrov::Point &b)
{
  return a.y < b.y;
}

bool petrov::comparePoints(const petrov::Point &a, const petrov::Point &b)
{
  return a.x < b.x || (a.x == b.x && a.y < b.y);
}

petrov::Point petrov::translatePoint(const petrov::Point &p, int dx, int dy)
{
  return {p.x - dx, p.y - dy};
}

petrov::Point petrov::swapPointXY(const petrov::Point &p)
{
  return {p.y, p.x};
}

double petrov::crossTerm(const std::vector< petrov::Point > &pts, std::size_t i, std::size_t n)
{
  std::size_t j = (i + 1) % n;
  return static_cast< double >(pts[i].x) * static_cast< double >(pts[j].y)
    - static_cast< double >(pts[j].x) * static_cast< double >(pts[i].y);
}

double petrov::calculateArea(const petrov::Polygon &poly)
{
  const std::vector< Point > &pts = poly.points;
  std::size_t n = pts.size();
  if (n < 3) {
    return 0.0;
  }
  std::vector< std::size_t > indices(n);
  std::iota(indices.begin(), indices.end(), 0);
  std::vector< double > terms(n);
  std::transform(
    indices.begin(),
    indices.end(),
    terms.begin(),
    std::bind(crossTerm, std::cref(pts), std::placeholders::_1, n)
  );
  double sum = std::accumulate(terms.begin(), terms.end(), 0.0);
  return std::abs(sum) / 2.0;
}

petrov::Polygon petrov::swapCoordinates(const petrov::Polygon &poly)
{
  Polygon res;
  res.points.reserve(poly.points.size());
  std::transform(poly.points.begin(), poly.points.end(), std::back_inserter(res.points), swapPointXY);
  return res;
}

std::vector< petrov::Point > petrov::normalize(const petrov::Polygon &poly)
{
  std::vector< Point > pts = poly.points;
  if (pts.empty()) {
    return pts;
  }
  int minX = std::min_element(pts.begin(), pts.end(), compareByX)->x;
  int minY = std::min_element(pts.begin(), pts.end(), compareByY)->y;
  std::transform(
    pts.begin(),
    pts.end(),
    pts.begin(),
    std::bind(translatePoint, std::placeholders::_1, minX, minY)
  );
  std::sort(pts.begin(), pts.end(), comparePoints);
  return pts;
}

bool petrov::isPermutationOf(const petrov::Polygon &a, const petrov::Polygon &b)
{
  if (a.points.size() != b.points.size()) {
    return false;
  }
  return normalize(a) == normalize(b);
}
