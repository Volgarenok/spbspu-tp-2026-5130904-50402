#include "polygon.hpp"
#include "delimiterio.hpp"

namespace muhamadiarov
{
  std::istream& operator>>(std::istream& in, Point& point)
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
    in >> point.x;
    in >> DelimiterIO{';'};
    if (!in)
    {
      return in;
    }
    in >> point.y;
    return in >> DelimiterIO{')'};
  }

  std::istream& operator>>(std::istream& in, Polygon& polygon)
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
    polygon.points.resize(count);
    using iit_t = std::istream_iterator<Point>;
    std::copy_n(iit_t(in), count, polygon.points.begin());
    if (!in)
    {
      polygon.points.clear();
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  bool operator==(const Point& lhs, const Point& rhs)
  {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
  }
}

