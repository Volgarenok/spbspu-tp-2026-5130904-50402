#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

namespace pozdnyakov
{

  struct Point
  {
    int x, y;
  };

  bool operator==(const Point &lhs, const Point &rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  bool operator!=(const Point &lhs, const Point &rhs)
  {
    return !(lhs == rhs);
  }

  struct Polygon
  {
    std::vector< Point > points;
  };

  bool operator==(const Polygon &lhs, const Polygon &rhs)
  {
    return lhs.points == rhs.points;
  }

  struct DelimiterIO
  {
    char expected;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.expected)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, Point &dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    Point temp{0, 0};
    in >> DelimiterIO{'('} >> temp.x >> DelimiterIO{';'} >> temp.y >> DelimiterIO{')'};

    if (in) {
      dest = temp;
    }
    return in;
  }

  struct PointReader
  {
    std::istream &in;
    Point operator()() const
    {
      Point p{0, 0};
      in >> p;
      return p;
    }
  };

  std::istream &operator>>(std::istream &in, Polygon &dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }

    size_t size = 0;
    in >> size;
    if (!in) {
      return in;
    }

    Polygon temp;

    std::generate_n(std::back_inserter(temp.points), size, PointReader{in});

    if (in) {
      dest = temp;
    }
    return in;
  }

}
