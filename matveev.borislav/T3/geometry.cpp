#include "geometry.hpp"

#include <algorithm>
#include <cstddef>
#include <istream>
#include <iterator>
#include <stdexcept>

#include <ioFormat.hpp>

namespace
{
  struct PointReader
  {
    std::istream& in;

    matveev::Point operator()() const
    {
      matveev::Point point;
      in >> point;

      if (!in)
      {
        throw std::logic_error("invalid point");
      }

      return point;
    }
  };
}

std::istream& matveev::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  int x = 0;
  int y = 0;

  in >> DelimiterIO{ '(' };
  in >> x;
  in >> DelimiterIO{ ';' };
  in >> y;
  in >> DelimiterIO{ ')' };

  if (in)
  {
    point = Point{ x, y };
  }

  return in;
}

std::istream& matveev::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  std::size_t count = 0;
  in >> count;

  if (!in || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  Polygon input;
  input.points.reserve(count);

  try
  {
    std::generate_n(std::back_inserter(input.points), count, PointReader{ in });
  }
  catch (const std::logic_error&)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (in)
  {
    polygon = input;
  }

  return in;
}
