#include "polygon.hpp"
#include "delimiterio.hpp"

namespace muh = muhamadiarov;

std::istream& muh::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'('};
  if (!in)
  {
    return in;
  }
  in >> point.x_;
  in >> DelimiterIO{';'};
  if (!in)
  {
    return in;
  }
  in >> point.y_;
  return in >> DelimiterIO{')'};
}

std::istream& muh::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count;
  if (!(in >>count) || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  using iit_t = std::istream_iterator< Point >;
  std::copy(iit_t{in}, iit_t{}, std::back_inserter(polygon.points_));
  if (polygon.points_.size() != count)
  {
    polygon.points_.clear();
    in.setstate(std::ios::failbit);
  }
  return in;
}

