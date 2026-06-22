#include "Parser.hpp"
#include <algorithm>
#include <iterator>
#include <vector>

novikov::IOGuard::IOGuard(std::basic_ios< char >& s):
    s_(s),
    width_(s.width()),
    precision_(s.precision()),
    fmt_(s.flags()),
    fill_(s.fill())
{}

novikov::IOGuard::~IOGuard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

std::istream& novikov::operator>>(std::istream& in, Point& pt)
{
  in >> DelimiterIO{' '} >> DelimiterIO{'('} >> pt.x >> DelimiterIO{';'} >> pt.y >> DelimiterIO{')'};
  return in;
}

std::istream& novikov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char c = 0;
  if (in.get(c))
  {
    if (c != dest.exp)
    {
      in.putback(c);
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::istream& novikov::operator>>(std::istream& in, PolygonIO&& dest)
{
  IOGuard g(in);
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }

  size_t count = 0;
  if (!(in >> count))
  {
    return in;
  }

  if (count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  in >> std::noskipws;

  std::vector< novikov::Point > temp_points;
  temp_points.reserve(count);

  std::copy_n(std::istream_iterator< novikov::Point >(in), count, std::back_inserter(temp_points));

  if (in)
  {
    dest.p.points = std::move(temp_points);
  }
  return in;
}

std::istream& novikov::operator>>(std::istream& in, Polygon& p)
{
  return in >> PolygonIO{p};
}

novikov::plg_t novikov::parse(std::istream& in)
{
  novikov::plg_t data;
  using iit_t = std::istream_iterator< novikov::Polygon >;
  std::copy(iit_t{in}, iit_t{}, std::back_inserter(data));
  return data;
}
