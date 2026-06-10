#include "polygon.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>
#include "../common/delim.hpp"
#include "../common/ioguard.hpp"

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
