#ifndef POLYGON_ALGORITHMS_HPP
#define POLYGON_ALGORITHMS_HPP

#include <cstddef>
#include <vector>

#include "geometry.hpp"

namespace matveev
{
  struct Frame
  {
    Point left_bottom;
    Point right_top;
  };

  struct Edge
  {
    Point first;
    Point second;
  };

  namespace detail
  {
    class TriangleAreaGenerator
    {
    public:
      TriangleAreaGenerator(const Point& base, std::vector< Point >::const_iterator current);

      double operator()();

    private:
      const Point& base_;
      std::vector< Point >::const_iterator current_;
    };

    class EdgeGenerator
    {
    public:
      explicit EdgeGenerator(const std::vector< Point >& points);

      Edge operator()();

    private:
      const std::vector< Point >& points_;
      std::size_t index_;
    };

    class IntersectWithEdges
    {
    public:
      explicit IntersectWithEdges(const std::vector< Edge >& edges);

      bool operator()(const Edge& edge) const;

    private:
      const std::vector< Edge >& edges_;
    };

    class PointOnEdge
    {
    public:
      explicit PointOnEdge(const Point& point);

      bool operator()(const Edge& edge) const;

    private:
      const Point& point_;
    };

    class RayCrossCounter
    {
    public:
      explicit RayCrossCounter(const Point& point);

      bool operator()(const Edge& edge) const;

    private:
      const Point& point_;
    };

    class PointInsidePolygon
    {
    public:
      explicit PointInsidePolygon(const Polygon& polygon);

      bool operator()(const Point& point) const;

    private:
      const Polygon& polygon_;
    };
  }

  double getTriangleArea(const Point& first, const Point& second, const Point& third);
  double getPolygonArea(const Polygon& polygon);

  std::vector< Edge > getEdges(const Polygon& polygon);
  bool isSegmentsIntersect(const Edge& first, const Edge& second);
  bool isPolygonIntersect(const Polygon& first, const Polygon& second);
  bool isPointInPolygon(const Point& point, const Polygon& polygon);
  bool isPointOnPolygon(const Point& point, const Polygon& polygon);

  Frame getFrame(const Polygon& polygon);
  Frame getFrame(const std::vector< Polygon >& polygons);
  bool isPointInFrame(const Frame& frame, const Point& point);
  bool isPolygonInFrame(const Frame& frame, const Polygon& polygon);
}

#endif
