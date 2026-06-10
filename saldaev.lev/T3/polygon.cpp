#include "polygon.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>
#include "../common/delim.hpp"
#include "../common/ioguard.hpp"

namespace
{
  double crossTerm(const std::vector< saldaev::Point > &pts, size_t i, size_t n)
  {
    size_t j = (i + 1) % n;
    return pts[i].x * pts[j].y - pts[j].x * pts[i].y;
  }
}

std::istream &saldaev::operator>>(std::istream &in, Point &point)
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

std::istream &saldaev::operator>>(std::istream &in, Polygon &polygon)
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

double saldaev::calcArea(const Polygon &polygon)
{
  size_t n = polygon.points.size();
  if (n < 3) {
    return 0.0;
  }
  std::vector< size_t > idxs(n);
  std::iota(idxs.begin(), idxs.end(), 0);
  std::vector< double > terms(n);
  auto func = std::bind(crossTerm, std::cref(polygon.points), std::placeholders::_1, n);
  std::transform(idxs.begin(), idxs.end(), terms.begin(), func);
  double sum = std::accumulate(terms.begin(), terms.end(), 0.0, std::plus< double >());
  return std::abs(sum) / 2.0;
}
