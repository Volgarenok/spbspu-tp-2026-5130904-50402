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
  struct Frame {
    samarin::Point min;
    samarin::Point max;
  };

  long long crossProduct(const samarin::Point &a, const samarin::Point &b)
  {
    return static_cast< long long >(a.x) * b.y - static_cast< long long >(b.x) * a.y;
  }

  Frame getFrame(const samarin::Polygon &polygon)
  {
    const std::vector< samarin::Point > &points = polygon.points;
    const auto byX = [](const samarin::Point &a, const samarin::Point &b)
    {
      return a.x < b.x;
    };
    const auto byY = [](const samarin::Point &a, const samarin::Point &b)
    {
      return a.y < b.y;
    };
    Frame frame{};
    frame.min.x = std::min_element(points.begin(), points.end(), byX)->x;
    frame.max.x = std::max_element(points.begin(), points.end(), byX)->x;
    frame.min.y = std::min_element(points.begin(), points.end(), byY)->y;
    frame.max.y = std::max_element(points.begin(), points.end(), byY)->y;
    return frame;
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

bool samarin::hasRightAngle(const Polygon &polygon)
{
  const std::vector< Point > &points = polygon.points;
  const std::size_t size = points.size();
  std::vector< std::size_t > indices(size);
  std::iota(indices.begin(), indices.end(), static_cast< std::size_t >(0));
  const auto rightAt = [&points, size](std::size_t i)
  {
    const Point &prev = points[(i + size - 1) % size];
    const Point &cur = points[i];
    const Point &next = points[(i + 1) % size];
    const long long dot = static_cast< long long >(prev.x - cur.x) * (next.x - cur.x)
      + static_cast< long long >(prev.y - cur.y) * (next.y - cur.y);
    return dot == 0;
  };
  return std::any_of(indices.begin(), indices.end(), rightAt);
}

bool samarin::intersects(const Polygon &lhs, const Polygon &rhs)
{
  const Frame a = getFrame(lhs);
  const Frame b = getFrame(rhs);
  return (a.min.x <= b.max.x) && (b.min.x <= a.max.x)
    && (a.min.y <= b.max.y) && (b.min.y <= a.max.y);
}
