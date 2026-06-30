#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include "commands.hpp"
#include "IOGuard.hpp"
#include "getArea.hpp"

namespace
{
  void maxArea(std::ostream&, const novikov::plg_t&);
  void maxVertexes(std::ostream&, const novikov::plg_t&);

  struct AreaComparator
  {
  public:
    bool operator()(const novikov::Polygon& a, const novikov::Polygon& b) const
    {
      return getPolygonArea(a) < getPolygonArea(b);
    }
  };

  struct VertexComparator
  {
  public:
    bool operator()(const novikov::Polygon& a, const novikov::Polygon& b) const
    {
      return a.points.size() < b.points.size();
    }
  };

  void maxArea(std::ostream& out, const novikov::plg_t& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("AREA requires at least one polygon in the dataset");
    }
    auto it = std::max_element(polygons.begin(), polygons.end(), AreaComparator{});
    out << getPolygonArea(*it);
  }

  void maxVertexes(std::ostream& out, const novikov::plg_t& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("VERTEXES requires at least one polygon in the dataset");
    }
    auto it = std::max_element(polygons.begin(), polygons.end(), VertexComparator{});
    out << it->points.size();
  }
}

void novikov::max(std::istream& in, std::ostream& out, novikov::plg_t& polygons)
{
  std::string subCmd;
  if (!(in >> subCmd))
  {
    throw std::logic_error("Missing or unreachable subcommand argument");
  }

  novikov::IOGuard g(out);
  out << std::fixed << std::setprecision(1);

  using subCmd_t = void (*)(std::ostream&, const plg_t&);
  std::unordered_map< std::string, subCmd_t > subCmds;
  subCmds["AREA"] = maxArea;
  subCmds["VERTEXES"] = maxVertexes;

  if (subCmds.count(subCmd))
  {
    subCmds[subCmd](out, polygons);
  }
  else
  {
    throw std::logic_error("Invalid subcommand");
  }
}
