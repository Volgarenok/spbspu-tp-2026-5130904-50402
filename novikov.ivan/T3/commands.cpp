#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>
#include "Parser.hpp"

namespace
{
  struct CrossProductFunctor
  {
  public:
    explicit CrossProductFunctor(const std::vector< novikov::Point >& pointsVec):
        points(pointsVec)
    {}

    double operator()(size_t i) const
    {
      size_t next = (i + 1) % points.size();
      return static_cast< double >(points[i].x * points[next].y - points[next].x * points[i].y);
    }

  private:
    const std::vector< novikov::Point >& points;
  };

  double getPolygonArea(const novikov::Polygon& p)
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

  struct EvenAreaTransformer
  {
  public:
    double operator()(const novikov::Polygon& p) const
    {
      if (p.points.size() % 2 == 0)
      {
        return getPolygonArea(p);
      }
      return 0;
    }
  };

  struct OddAreaTransformer
  {
  public:
    double operator()(const novikov::Polygon& p) const
    {
      if (p.points.size() % 2 != 0)
      {
        return getPolygonArea(p);
      }
      return 0;
    }
  };

  struct NumAreaTransformer
  {
  public:
    explicit NumAreaTransformer(size_t targetValue):
        target(targetValue)
    {}

    double operator()(const novikov::Polygon& p) const
    {
      if (p.points.size() == target)
      {
        return getPolygonArea(p);
      }
      return 0;
    }

  private:
    size_t target;
  };

  struct TotalAreaTransformer
  {
  public:
    double operator()(const novikov::Polygon& p) const
    {
      return getPolygonArea(p);
    }
  };

  bool isCharDigit(char c)
  {
    return std::isdigit(static_cast< unsigned char >(c)) != 0;
  }

  void areaEven(std::ostream& out, const novikov::plg_t& polygons)
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), EvenAreaTransformer{});
    out << std::accumulate(areas.begin(), areas.end(), 0.0) << "\n";
  }

  void areaOdd(std::ostream& out, const novikov::plg_t& polygons)
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), OddAreaTransformer{});
    out << std::accumulate(areas.begin(), areas.end(), 0.0) << "\n";
  }

  void areaMean(std::ostream& out, const novikov::plg_t& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("MEAN requires at least one polygon in the dataset");
    }
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), TotalAreaTransformer{});
    out << (std::accumulate(areas.begin(), areas.end(), 0.0) / static_cast< double >(polygons.size())) << "\n";
  }

  void areaNum(std::ostream& out, const novikov::plg_t& polygons, size_t num)
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), NumAreaTransformer{num});
    out << std::accumulate(areas.begin(), areas.end(), 0.0) << "\n";
  }
}

void novikov::area(std::istream& in, std::ostream& out, novikov::plg_t& polygons)
{
  std::string subCmd;
  if (!(in >> subCmd))
  {
    throw std::logic_error("Invalid subcommand argument");
  }

  novikov::IOGuard g(out);
  out << std::fixed << std::setprecision(1);

  if (subCmd == "EVEN")
  {
    areaEven(out, polygons);
  }
  else if (subCmd == "ODD")
  {
    areaOdd(out, polygons);
  }
  else if (subCmd == "MEAN")
  {
    areaMean(out, polygons);
  }
  else if (std::all_of(subCmd.begin(), subCmd.end(), isCharDigit))
  {
    size_t num = std::stoull(subCmd);
    if (num >= 3)
    {
      areaNum(out, polygons, num);
    }
    else
    {
      throw std::logic_error("Invalid number of vertices");
    }
  }
  else
  {
    throw std::logic_error("Invalid subcommand");
  }
}
