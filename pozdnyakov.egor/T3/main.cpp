#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
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

  struct ShoelaceTerm
  {
    double operator()(const Point &p1, const Point &p2) const
    {
      return static_cast< double >(p1.x) * p2.y - static_cast< double >(p1.y) * p2.x;
    }
  };

  inline double getArea(const Polygon &poly)
  {
    if (poly.points.size() < 3)
      return 0.0;

    double sum = std::inner_product(poly.points.begin(), poly.points.end() - 1, poly.points.begin() + 1, 0.0,
                                    std::plus< double >(), ShoelaceTerm{});

    sum += ShoelaceTerm{}(poly.points.back(), poly.points.front());

    return std::abs(sum) / 2.0;
  }

  struct CheckPermutation
  {
    const Polygon &target;
    bool operator()(const Polygon &p) const
    {
      if (p.points.size() != target.points.size())
        return false;
      return std::is_permutation(p.points.begin(), p.points.end(), target.points.begin(), target.points.end());
    }
  };

  struct Segment
  {
    Point a, b;
  };

  struct MakeSegment
  {
    Segment operator()(const Point &a, const Point &b) const
    {
      return Segment{a, b};
    }
  };

  inline std::vector< Segment > getSegments(const Polygon &p)
  {
    std::vector< Segment > segs;
    if (p.points.size() < 2)
      return segs;
    std::transform(p.points.begin(), p.points.end() - 1, p.points.begin() + 1, std::back_inserter(segs), MakeSegment{});
    segs.push_back(MakeSegment{}(p.points.back(), p.points.front()));
    return segs;
  }

  inline int orientation(const Point &p, const Point &q, const Point &r)
  {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
      return 0;
    return (val > 0) ? 1 : 2;
  }

  inline bool onSegment(const Point &p, const Point &q, const Point &r)
  {
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y)
           && q.y >= std::min(p.y, r.y);
  }

  struct CheckSegmentIntersect
  {
    Segment s1;
    bool operator()(const Segment &s2) const
    {
      int o1 = orientation(s1.a, s1.b, s2.a);
      int o2 = orientation(s1.a, s1.b, s2.b);
      int o3 = orientation(s2.a, s2.b, s1.a);
      int o4 = orientation(s2.a, s2.b, s1.b);

      if (o1 != o2 && o3 != o4)
        return true;
      if (o1 == 0 && onSegment(s1.a, s2.a, s1.b))
        return true;
      if (o2 == 0 && onSegment(s1.a, s2.b, s1.b))
        return true;
      if (o3 == 0 && onSegment(s2.a, s1.a, s2.b))
        return true;
      if (o4 == 0 && onSegment(s2.a, s1.b, s2.b))
        return true;

      return false;
    }
  };

  struct CheckAnySegmentIntersect
  {
    const std::vector< Segment > &segs2;
    bool operator()(const Segment &s1) const
    {
      return std::any_of(segs2.begin(), segs2.end(), CheckSegmentIntersect{s1});
    }
  };

  struct RayIntersectCheck
  {
    Point p;
    bool operator()(const Segment &seg) const
    {
      if ((seg.a.y > p.y) != (seg.b.y > p.y)) {
        double intersectX =
            seg.a.x
            + static_cast< double >(p.y - seg.a.y) * (seg.b.x - seg.a.x) / static_cast< double >(seg.b.y - seg.a.y);
        return static_cast< double >(p.x) < intersectX;
      }
      return false;
    }
  };

  struct CheckPointInsidePolygon
  {
    const std::vector< Segment > &polySegs;
    bool operator()(const Point &pt) const
    {
      size_t intersections = std::count_if(polySegs.begin(), polySegs.end(), RayIntersectCheck{pt});
      return (intersections % 2) == 1;
    }
  };

  struct CheckPolygonsIntersect
  {
    const Polygon &p1;
    bool operator()(const Polygon &p2) const
    {
      auto segs1 = getSegments(p1);
      auto segs2 = getSegments(p2);

      if (std::any_of(segs1.begin(), segs1.end(), CheckAnySegmentIntersect{segs2}))
        return true;

      if (std::any_of(p1.points.begin(), p1.points.end(), CheckPointInsidePolygon{segs2}))
        return true;

      if (std::any_of(p2.points.begin(), p2.points.end(), CheckPointInsidePolygon{segs1}))
        return true;

      return false;
    }
  };

}
