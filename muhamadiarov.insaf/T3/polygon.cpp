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
  if (!(in >> count) || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  polygon.points_.resize(count);
  using iit_t = std::istream_iterator<Point>;
  std::copy_n(iit_t(in), count, polygon.points_.begin());
  if (!in)
  {
    polygon.points_.clear();
    in.setstate(std::ios::failbit);
  }

  Point p;
  if (in >> p)
  {
    in.setstate(std::ios::failbit);
    polygon.points_.clear();
  }
  return in;
}

bool muh::operator==(const Point& lhs, const Point& rhs)
{
  return (lhs.x_ == rhs.x_) && (lhs.y_ == rhs.y_);
}

