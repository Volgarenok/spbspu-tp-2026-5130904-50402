#include "commands.hpp"

namespace detail
{
  using namespace muhamadiarov;

  double getArea(const Polygon& p)
  {
    if (p.points.size() < 3)
    {
      return 0.0;
    }

    struct Accumulator
    {
      double operator()(const Point& a, const Point& b)
      {
        return static_cast< double >(a.x * b.y - a.y * b.x);
      }
    };

    std::vector< double > crossProducts;
    std::transform(
      p.points.begin(),
      p.points.end() - 1,
      p.points.begin() + 1,
      std::back_inserter(crossProducts), Accumulator()
    );

    crossProducts.push_back(Accumulator()(p.points.back(), p.points.front()));
    const double sum = std::accumulate(crossProducts.begin(), crossProducts.end(), 0.0);
    return std::abs(sum) / 2.0;
  }

  double sumArea(const std::vector< Polygon >& p)
  {
    std::vector< double > areas;
    std::transform(p.begin(), p.end(), std::back_inserter(areas), getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  bool isOdd(const Polygon& p)
  {
    return (p.points.size() % 2) != 0;
  }

  bool isEven(const Polygon& p)
  {
    return (p.points.size() % 2) == 0;
  }

  bool isCountOfVertices(const Polygon& p, size_t n)
  {
    return n == p.points.size();
  }

  size_t getVerticesCount(const Polygon& p)
  {
    return p.points.size();
  }

  bool isRightAngle(const Point& a, const Point& b, const Point& c)
  {
    double dx1 = b.x - a.x;
    double dy1 = b.y - a.y;
    double dx2 = c.x - b.x;
    double dy2 = c.y - b.y;
    double result = dx1 * dx2 + dy1 * dy2;
    return std::abs(result) < 1e-9;
  }

  struct CheckAngleFunctor
  {
    const Polygon& polygon;

    CheckAngleFunctor(const Polygon& polygon):
      polygon(polygon)
    {}

    bool operator()(const Point& current) const
    {
      auto it = std::find_if(
        polygon.points.begin(),
        polygon.points.end(),
        std::bind(std::equal_to<Point>(), std::placeholders::_1, current)
      );

      if (it == polygon.points.end())
      {
        return false;
      }

      size_t idx = std::distance(polygon.points.begin(), it);

      size_t prev = (idx == 0) ? polygon.points.size() - 1 : idx - 1;
      size_t next = (idx + 1) % polygon.points.size();

      return isRightAngle(
        polygon.points[prev],
        polygon.points[idx],
        polygon.points[next]);
    }
  };

  bool hasRightAngle(const Polygon& p)
  {
    if (p.points.size() < 3)
    {
      return false;
    }

    std::vector< bool > cHas(p.points.size());
    CheckAngleFunctor angleChecker(p);
    std::transform(p.points.begin(), p.points.end(), cHas.begin(), angleChecker);

    return std::any_of(
      cHas.begin(),
      cHas.end(),
      std::bind(std::equal_to< bool >(), std::placeholders::_1, true)
    );
  }

  struct Segment
  {
    Point p1;
    Point p2;
  };

  Segment getSegment(const Point& p1, const Point& p2)
  {
    Segment seg;
    seg.p1 = p1;
    seg.p2 = p2;
    return seg;
  }

  struct GetSegmentsFunctor
  {
    std::vector<Segment> operator()(const Polygon& polygon) const
    {
      std::vector<Segment> segments(polygon.points.size());
      std::transform(
        polygon.points.begin(),
        polygon.points.end() - 1,
        polygon.points.begin() + 1,
        segments.begin(),
        getSegment
      );

      segments.back().p1 = polygon.points.back();
      segments.back().p2 = polygon.points.front();

      return segments;
    }
  };

  double crossProduct(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
  }

  bool onSegment(const Point& a, const Point& b, const Point& c) {
    bool b1 = c.x <= std::max(a.x, b.x) && c.x >= std::min(a.x, b.x);
    bool b2 = c.y <= std::max(a.y, b.y) && c.y >= std::min(a.y, b.y);
    return b1 && b2;
  }

  bool doIntersect(const Segment& s1, const Segment& s2) {
    Point p1 = s1.p1;
    Point p2 = s1.p2;
    Point p3 = s2.p1;
    Point p4 = s2.p2;

    double d1 = crossProduct(p3, p4, p1);
    double d2 = crossProduct(p3, p4, p2);
    double d3 = crossProduct(p1, p2, p3);
    double d4 = crossProduct(p1, p2, p4);


    bool b1 = (d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0);
    bool b2 = (d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0);
    if (b1 && b2)
    {
      return true;
    }

    if (d1 == 0 && onSegment(p3, p4, p1))
    {
      return true;
    }
    if (d2 == 0 && onSegment(p3, p4, p2))
    {
      return true;
    }
    if (d3 == 0 && onSegment(p1, p2, p3))
    {
      return true;
    }
    if (d4 == 0 && onSegment(p1, p2, p4))
    {
      return true;
    }

    return false;
  }

  struct AnySegmentIntersectsFunctor
  {
    const Segment& seg;

    AnySegmentIntersectsFunctor(const Segment& seg):
      seg(seg)
    {}

    bool operator()(const Segment& other) const
    {
      return doIntersect(other, seg);
    }
  };

  struct SegmentIntersectsPolygonFunctor
  {
    const std::vector< Segment >& segs;

    SegmentIntersectsPolygonFunctor(const std::vector< Segment >& segs):
      segs(segs)
    {}

    bool operator()(const Segment& seg) const
    {
      return std::any_of(
        segs.begin(),
        segs.end(),
        AnySegmentIntersectsFunctor(seg)
      );
    }
  };

  struct PointInPolygonFunctor
  {
    const std::vector< Segment >& segs;

    PointInPolygonFunctor(const std::vector< Segment >& segs):
      segs(segs)
    {}

    bool operator()(const Point& p) const
    {
      Point far{ 1000000, p.y };
      Segment ray{ p, far };
      size_t cnt = std::count_if(
        segs.begin(),
        segs.end(),
        std::bind(doIntersect, std::placeholders::_1, ray)
      );
      return (cnt % 2) == 1;
    }
  };

  struct PolygonIntersectionFunctor
  {
    const Polygon& p;

    PolygonIntersectionFunctor(const Polygon& polygon):
      p(polygon)
    {}

    bool operator()(const Polygon& other) const
    {
      GetSegmentsFunctor gs;
      std::vector< Segment > segsP  = gs(p);
      std::vector< Segment > segsOther = gs(other);

      bool edgesIntersect = std::any_of(
        segsP.begin(),
        segsP.end(),
        SegmentIntersectsPolygonFunctor(segsOther)
      );
      if (edgesIntersect)
      {
        return true;
      }

      bool pInsideOther = std::any_of(
        p.points.begin(),
        p.points.end(),
        PointInPolygonFunctor(segsOther)
      );
      if (pInsideOther)
      {
        return true;
      }

      return std::any_of(
        other.points.begin(),
        other.points.end(),
        PointInPolygonFunctor(segsP)
      );
    }
  };
}

namespace muh = muhamadiarov;

void muh::area(
  std::istream& in,
  std::ostream& out,
  const std::vector< Polygon >& polygons
)
{
  std::string command;
  if (!(in >> command))
  {
    throw std::runtime_error("error input");
  }

  IOguard ioguard(out);
  out << std::fixed << std::setprecision(1);

  std::vector< Polygon > polygonsIf;
  if (command == "EVEN")
  {
    std::copy_if(
      polygons.begin(),
      polygons.end(),
      std::back_inserter(polygonsIf),
      detail::isEven
    );
    out << detail::sumArea(polygonsIf) << '\n';
  }
  else if (command == "ODD")
  {
    std::copy_if(
      polygons.begin(),
      polygons.end(),
      std::back_inserter(polygonsIf),
      detail::isOdd
    );
    out << detail::sumArea(polygonsIf) << '\n';
  }
  else if (command == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::logic_error("Empty collection");
    }
    out << detail::sumArea(polygons) / static_cast< double >(polygons.size()) << '\n';
  }
  else
  {
    const size_t count = std::stoull(command);
    if (count <  3)
    {
      throw std::logic_error("Invalid argument");
    }

    std::copy_if(
      polygons.begin(),
      polygons.end(),
      std::back_inserter(polygonsIf),
      std::bind(detail::isCountOfVertices, std::placeholders::_1, count)
    );
    out << detail::sumArea(polygonsIf) << '\n';
  }
}

void muh::max(
  std::istream& in,
  std::ostream& out,
  const std::vector< Polygon >& polygons
)
{
  std::string command;
  if (!(in >> command))
  {
    throw std::runtime_error("error input");
  }

  if (polygons.empty())
  {
    throw std::logic_error("Empty collection");
  }

  if (command == "AREA")
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), detail::getArea);
    auto it = std::max_element(areas.begin(), areas.end());
    if (it != areas.end())
    {
      IOguard ioguard(out);
      out << std::fixed << std::setprecision(1) << *it << '\n';
    }
  }
  else if (command == "VERTEXES")
  {
    std::vector< size_t > counts;
    std::transform(
      polygons.begin(),
      polygons.end(),
      std::back_inserter(counts),
      detail::getVerticesCount
    );
    auto it = std::max_element(counts.begin(), counts.end());
    if (it != counts.end())
    {
      out << *it << '\n';
    }
  }
  else
  {
    throw std::logic_error("Invalid command");
  }
}

void muh::min(
  std::istream& in,
  std::ostream& out,
  const std::vector< Polygon >& polygons
)
{
  std::string command;
  if (!(in >> command))
  {
    throw std::runtime_error("error input");
  }

  if (polygons.empty())
  {
    throw std::logic_error("Empty collection");
  }

  if (command == "AREA")
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), detail::getArea);
    auto it = std::min_element(areas.begin(), areas.end());
    if (it != areas.end())
    {
      IOguard ioguard(out);
      out << std::fixed << std::setprecision(1) << *it << '\n';
    }
  }
  else if (command == "VERTEXES")
  {
    std::vector< size_t > counts;
    std::transform(
      polygons.begin(),
      polygons.end(),
      std::back_inserter(counts),
      detail::getVerticesCount
    );
    auto it = std::min_element(counts.begin(), counts.end());
    if (it != counts.end())
    {
      out << *it << '\n';
    }
  }
  else
  {
    throw std::logic_error("Invalid command");
  }
}

void muh::count(
  std::istream& in,
  std::ostream& out,
  const std::vector< Polygon >& polygons
)
{
  std::string command;
  if (!(in >> command))
  {
    throw std::runtime_error("error input");
  }
  if (command == "EVEN")
  {
    out << std::count_if(polygons.begin(), polygons.end(), detail::isEven) << '\n';
  }
  else if (command == "ODD")
  {
    out << std::count_if(polygons.begin(), polygons.end(), detail::isOdd) << '\n';
  }
  else
  {
    const size_t count  = std::stoull(command);
    if (count < 3)
    {
      throw std::logic_error("Invalid command");
    }
    out << std::count_if(
      polygons.begin(),
      polygons.end(),
      std::bind(detail::isCountOfVertices, std::placeholders::_1, count)
    );
     out << '\n';
  }
}

void muh::rightshapes(
  std::istream&,
  std::ostream& out,
  const std::vector< Polygon >& polygons
)
{
  out << std::count_if(polygons.begin(), polygons.end(), detail::hasRightAngle) << '\n';
}

void muh::intersections(
  std::istream& in,
  std::ostream& out,
  const std::vector< Polygon >& polygons
)
{
  Polygon inPoligon;
  if (!(in >> inPoligon))
  {
    throw std::runtime_error("Error input");
  }

  in >> std::ws;
  char next = in.peek();
  if (next == '(' || next == '-' || (next >= '0' && next <= '9'))
  {
    throw std::runtime_error("Invalid polygon");
  }

  detail::PolygonIntersectionFunctor checker{inPoligon};
  out << std::count_if(polygons.begin(), polygons.end(), checker) << '\n';
}
