#include "Polygon.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <istream>
#include <iterator>
#include <numeric>
#include <vector>
#include "DelimiterIO.hpp"

namespace {
  long long crossProduct(const samarin::Point &a, const samarin::Point &b)
  {
    return static_cast< long long >(a.x) * b.y - static_cast< long long >(b.x) * a.y;
  }

  bool restOfLineIsBlank(std::istream &in)
  {
    int next = in.peek();
    while (next == ' ' || next == '\t') {
      in.get();
      next = in.peek();
    }
    return (next == '\n') || (next == std::char_traits< char >::eof());
  }
}

std::istream &samarin::operator>>(std::istream &in, Point &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  Point input{0, 0};
  in >> DelimiterIO{'('} >> input.x >> DelimiterIO{';'} >> input.y >> DelimiterIO{')'};
  if (in) {
    dest = input;
  }
  return in;
}

std::istream &samarin::operator>>(std::istream &in, Polygon &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::size_t count = 0;
  in >> count;
  if (!in || (count < 3)) {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > input;
  input.reserve(count);
  std::copy_n(std::istream_iterator< Point >(in), count, std::back_inserter(input));
  if (in && (input.size() == count) && restOfLineIsBlank(in)) {
    dest.points = input;
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

double samarin::getArea(const Polygon &polygon)
{
  const std::vector< Point > &points = polygon.points;
  std::vector< long long > terms(points.size());
  std::transform(points.begin(), points.end() - 1, points.begin() + 1, terms.begin(), crossProduct);
  terms.back() = crossProduct(points.back(), points.front());
  const long long doubled = std::accumulate(terms.begin(), terms.end(), 0LL);
  return std::abs(static_cast< double >(doubled)) / 2.0;
}
