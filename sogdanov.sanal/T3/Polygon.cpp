#include "Polygon.hpp"
#include "IoUtils.hpp"
#include <algorithm>

namespace sogdanov {

  bool Point::operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  bool Polygon::operator==(const Polygon& other) const
  {
    if (points.size() != other.points.size()) {
      return false;
    }
    return std::equal(points.begin(), points.end(), other.points.begin());
  }

  std::istream& operator>>(std::istream& in, Point& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    Point temp;
    in >> DelimiterIO{'('} >> temp.x >> DelimiterIO{';'} >> temp.y >> DelimiterIO{')'};
    if (in) {
      dest = temp;
    }
    return in;
  }

  struct PointReader {
    std::istream& in;
    Point operator()() const
    {
      Point p;
      in >> p;
      return p;
    }
  };

  std::istream& operator>>(std::istream& in, Polygon& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    size_t count = 0;
    if (!(in >> count) || count < 3) {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::vector<Point> temp;
    std::generate_n(std::back_inserter(temp), count, PointReader{in});
    if (in) {
      dest.points = temp;
    }
    return in;
  }

}
