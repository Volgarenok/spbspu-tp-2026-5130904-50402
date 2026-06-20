#include "commands.hpp"

namespace muhamadiarov
{
  double getArea(const Polygon& p)
  {
    if (p.points_.size() < 3)
    {
      return 0.0;
    }

    struct Accumulator
    {
      double operator()(const Point& a, const Point& b)
      {
        return static_cast<double>(a.x_ * b.y_ - a.y_ * b.x_);
      }
    };

    std::vector< double > crossProducts;
    std::transform(
      p.points_.begin(),
      p.points_.end() - 1,
      p.points_.begin() + 1,
      std::back_inserter(crossProducts), Accumulator()
    );

    crossProducts.push_back(Accumulator()(p.points_.back(), p.points_.front()));
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
    return (p.points_.size() % 2) != 0;
  }

  bool isEven(const Polygon& p)
  {
    return (p.points_.size() % 2) == 0;
  }

  bool isCountOfVertices(const Polygon& p, size_t n)
  {
    return n == p.points_.size();
  }

  size_t getVerticesCount(const Polygon& p)
  {
    return p.points_.size();
  }

  bool isRightAngle(const Point& a, const Point& b, const Point& c)
  {
    double dx1 = b.x_ - a.x_;
    double dy1 = b.y_ - a.y_;
    double dx2 = c.x_ - b.x_;
    double dy2 = c.y_ - b.y_;
    double result = dx1 * dx2 + dy1 * dy2;
    return std::abs(result) < 1e-9;
  }

  struct CheckAngleFunctor
  {
    const Polygon& polygon_;

    CheckAngleFunctor(const Polygon& polygon):
      polygon_(polygon)
    {}

    bool operator()(const Point& current) const
    {
      auto it = std::find_if(
        polygon_.points_.begin(),
        polygon_.points_.end(),
        std::bind(std::equal_to<Point>(), std::placeholders::_1, current)
      );

      if (it == polygon_.points_.end())
      {
        return false;
      }

      size_t idx = std::distance(polygon_.points_.begin(), it);

      size_t prev = (idx == 0) ? polygon_.points_.size() - 1 : idx - 1;
      size_t next = (idx + 1) % polygon_.points_.size();

      return isRightAngle(
        polygon_.points_[prev],
        polygon_.points_[idx],
        polygon_.points_[next]);
    }
  };

  bool hasRightAngle(const Polygon& p)
  {
    if (p.points_.size() < 3)
    {
      return false;
    }

    std::vector< bool > cHas(p.points_.size());
    CheckAngleFunctor angleChecker(p);
    std::transform(p.points_.begin(), p.points_.end(), cHas.begin(), angleChecker);

    return std::any_of(
      cHas.begin(),
      cHas.end(),
      std::bind(std::equal_to< bool >(), std::placeholders::_1, true)
    );
  }

  struct Segment
  {
    Point p1_;
    Point p2_;
  };

  Segment getSegment(const Point& p1, const Point& p2)
  {
    Segment seg;
    seg.p1_ = p1;
    seg.p2_ = p2;
    return seg;
  }

  struct GetSegmentsFunctor
  {
    std::vector<Segment> operator()(const Polygon& polygon) const
    {
      std::vector<Segment> segments(polygon.points_.size());
      std::transform(
        polygon.points_.begin(),
        polygon.points_.end() - 1,
        polygon.points_.begin() + 1,
        segments.begin(),
        getSegment
      );

      segments.back().p1_ = polygon.points_.back();
      segments.back().p2_ = polygon.points_.front();

      return segments;
    }
  };

  double crossProduct(const Point& a, const Point& b, const Point& c) {
    return (b.x_ - a.x_) * (c.y_ - a.y_) - (b.y_ - a.y_) * (c.x_ - a.x_);
  }

  bool onSegment(const Point& a, const Point& b, const Point& c) {
    bool b1 = c.x_ <= std::max(a.x_, b.x_) && c.x_ >= std::min(a.x_, b.x_);
    bool b2 = c.y_ <= std::max(a.y_, b.y_) && c.y_ >= std::min(a.y_, b.y_);
    return b1 && b2;
  }

  bool doIntersect(const Segment& s1, const Segment& s2) {
    Point p1 = s1.p1_;
    Point p2 = s1.p2_;
    Point p3 = s2.p1_;
    Point p4 = s2.p2_;

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
    const Segment& seg_;

    AnySegmentIntersectsFunctor(const Segment& seg):
      seg_(seg)
    {}

    bool operator()(const Segment& other) const
    {
      return doIntersect(other, seg_);
    }
  };

  struct SegmentIntersectsPolygonFunctor
  {
    const std::vector< Segment >& segs_;

    SegmentIntersectsPolygonFunctor(const std::vector< Segment >& segs):
      segs_(segs)
    {}

    bool operator()(const Segment& seg) const
    {
      return std::any_of(
        segs_.begin(),
        segs_.end(),
        AnySegmentIntersectsFunctor(seg)
      );
    }
  };

  struct PointInPolygonFunctor
  {
    const std::vector< Segment >& segs_;

    PointInPolygonFunctor(const std::vector< Segment >& segs):
      segs_(segs)
    {}

    bool operator()(const Point& p) const
    {
      Point far{ 1000000, p.y_ };
      Segment ray{ p, far };
      size_t cnt = std::count_if(
        segs_.begin(),
        segs_.end(),
        std::bind(doIntersect, std::placeholders::_1, ray)
      );
      return (cnt % 2) == 1;
    }
  };

  struct PolygonIntersectionFunctor
  {
    const Polygon& p_;

    PolygonIntersectionFunctor(const Polygon& polygon):
      p_(polygon)
    {}

    bool operator()(const Polygon& other) const
    {
      GetSegmentsFunctor gs;
      std::vector< Segment > segsP  = gs(p_);
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
        p_.points_.begin(),
        p_.points_.end(),
        PointInPolygonFunctor(segsOther)
      );
      if (pInsideOther)
      {
        return true;
      }

      return std::any_of(
        other.points_.begin(),
        other.points_.end(),
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
      isEven
    );
    out << sumArea(polygonsIf) << '\n';
  }
  else if (command == "ODD")
  {
    std::copy_if(
      polygons.begin(),
      polygons.end(),
      std::back_inserter(polygonsIf),
      isOdd
    );
    out << sumArea(polygonsIf) << '\n';
  }
  else if (command == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::logic_error("Empty collection");
    }
    out << sumArea(polygons) / static_cast< double >(polygons.size()) << '\n';
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
      std::bind(isCountOfVertices, std::placeholders::_1, count)
    );
    out << sumArea(polygonsIf) << '\n';
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
    std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
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
      getVerticesCount
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
    std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
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
      getVerticesCount
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
    out << std::count_if(polygons.begin(), polygons.end(), isEven) << '\n';
  }
  else if (command == "ODD")
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOdd) << '\n';
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
      std::bind(isCountOfVertices, std::placeholders::_1, count)
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
  out << std::count_if(polygons.begin(), polygons.end(), hasRightAngle) << '\n';
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

  PolygonIntersectionFunctor checker{inPoligon};
  out << std::count_if(polygons.begin(), polygons.end(), checker) << '\n';
}
