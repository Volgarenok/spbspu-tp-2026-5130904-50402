#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>
#include "IOguard.hpp"
#include "commands.hpp"

namespace
{
  double getPolygonArea(const shirokov::Polygon&);
  void minArea(std::ostream&, const shirokov::plg_t&);
  void minVertexes(std::ostream&, const shirokov::plg_t&);

  struct CrossProductFunctor
  {
  public:
    explicit CrossProductFunctor(const std::vector< shirokov::Point >& pointsVec):
      points(pointsVec)
    {}

    double operator()(size_t i) const
    {
      size_t next = (i + 1) % points.size();
      return static_cast< double >(points[i].x * points[next].y - points[next].x * points[i].y);
    }

  private:
    const std::vector< shirokov::Point >& points;
  };

  struct AreaComparator
  {
  public:
    bool operator()(const shirokov::Polygon& a, const shirokov::Polygon& b) const
    {
      return getPolygonArea(a) < getPolygonArea(b);
    }
  };

  struct VertexComparator
  {
  public:
    bool operator()(const shirokov::Polygon& a, const shirokov::Polygon& b) const
    {
      return a.points.size() < b.points.size();
    }
  };

  double getPolygonArea(const shirokov::Polygon& p)
  {
    if (p.points.empty())
    {
      return 0;
    }
    std::vector< size_t > indices(p.points.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::vector< double > partialAreas(p.points.size());
    std::transform(indices.begin(), indices.end(), partialAreas.begin(), CrossProductFunctor{p.points});
    return std::abs(std::accumulate(partialAreas.begin(), partialAreas.end(), 0.0)) / 2;
  }

  void minArea(std::ostream& out, const shirokov::plg_t& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("AREA requires at least one polygon in the dataset");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), AreaComparator{});
    out << getPolygonArea(*it) << "\n";
  }

  void minVertexes(std::ostream& out, const shirokov::plg_t& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("VERTEXES requires at least one polygon in the dataset");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), VertexComparator{});
    out << it->points.size() << "\n";
  }
}

void shirokov::min(std::istream& in, std::ostream& out, shirokov::plg_t& polygons)
{
  std::string subCmd;
  if (!(in >> subCmd))
  {
    throw std::logic_error("Missing or unreachable subcommand argument");
  }

  shirokov::IOguard g(out);
  out << std::fixed << std::setprecision(1);

  if (subCmd == "AREA")
  {
    minArea(out, polygons);
  }
  else if (subCmd == "VERTEXES")
  {
    minVertexes(out, polygons);
  }
  else
  {
    throw std::logic_error("Unrecognized subcommand");
  }
}
