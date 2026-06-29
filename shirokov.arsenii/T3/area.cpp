#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include "IOguard.hpp"
#include "commands.hpp"
#include "get_area.hpp"

namespace
{
  template< typename Predicate >
  double calculateAreaIf(const shirokov::plg_t&, Predicate);
  void areaEven(std::ostream&, const shirokov::plg_t&);
  void areaOdd(std::ostream&, const shirokov::plg_t&);
  void areaMean(std::ostream&, const shirokov::plg_t&);
  void areaNum(std::ostream&, const shirokov::plg_t&, size_t);

  struct IsEvenVertices
  {
    bool operator()(const shirokov::Polygon& p) const
    {
      return p.points.size() % 2 == 0;
    }
  };

  struct IsOddVertices
  {
    bool operator()(const shirokov::Polygon& p) const
    {
      return p.points.size() % 2 != 0;
    }
  };

  struct IsTargetVertices
  {
  public:
    explicit IsTargetVertices(size_t targetValue):
      target(targetValue)
    {}

    bool operator()(const shirokov::Polygon& p) const
    {
      return p.points.size() == target;
    }

  private:
    size_t target;
  };

  struct AlwaysTrue
  {
    bool operator()(const shirokov::Polygon&) const
    {
      return true;
    }
  };

  template< typename Predicate >
  struct AreaTransformer
  {
  public:
    explicit AreaTransformer(Predicate predicate):
      pred(predicate)
    {}

    double operator()(const shirokov::Polygon& p) const
    {
      return pred(p) ? getPolygonArea(p) : 0.0;
    }

  private:
    Predicate pred;
  };

  template< typename Predicate >
  double calculateAreaIf(const shirokov::plg_t& polygons, Predicate pred)
  {
    std::vector< double > areas(polygons.size());

    std::transform(polygons.begin(), polygons.end(), areas.begin(), AreaTransformer< Predicate >(pred));

    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  void areaEven(std::ostream& out, const shirokov::plg_t& polygons)
  {
    out << calculateAreaIf(polygons, IsEvenVertices{});
  }

  void areaOdd(std::ostream& out, const shirokov::plg_t& polygons)
  {
    out << calculateAreaIf(polygons, IsOddVertices{});
  }

  void areaMean(std::ostream& out, const shirokov::plg_t& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("MEAN requires at least one polygon in the dataset");
    }
    double totalArea = calculateAreaIf(polygons, AlwaysTrue{});
    out << (totalArea / static_cast< double >(polygons.size()));
  }

  void areaNum(std::ostream& out, const shirokov::plg_t& polygons, size_t num)
  {
    out << calculateAreaIf(polygons, IsTargetVertices{num});
  }
}

void shirokov::area(std::istream& in, std::ostream& out, plg_t& polygons)
{
  std::string subCmd;
  if (!(in >> subCmd))
  {
    throw std::logic_error("Missing or unreachable subcommand argument");
  }

  IOguard g(out);
  out << std::fixed << std::setprecision(1);
  using subCmd_t = void (*)(std::ostream&, const plg_t&);
  std::unordered_map< std::string, subCmd_t > subCmds;
  subCmds["EVEN"] = areaEven;
  subCmds["ODD"] = areaOdd;
  subCmds["MEAN"] = areaMean;

  if (subCmds.count(subCmd))
  {
    subCmds[subCmd](out, polygons);
  }
  else
  {
    try
    {
      size_t idx = 0;
      size_t num = std::stoull(subCmd, &idx);

      if (idx != subCmd.length())
      {
        throw std::logic_error("Unrecognized subcommand");
      }

      if (num >= 3)
      {
        areaNum(out, polygons, num);
      }
      else
      {
        throw std::logic_error("Invalid number of vertices");
      }
    }
    catch (const std::invalid_argument&)
    {
      throw std::logic_error("Unrecognized subcommand");
    }
    catch (const std::out_of_range&)
    {
      throw std::logic_error("Invalid number of vertices");
    }
  }
}
