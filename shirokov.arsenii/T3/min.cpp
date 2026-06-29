#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include "IOguard.hpp"
#include "commands.hpp"
#include "get_area.hpp"

namespace
{
  void minArea(std::ostream&, const shirokov::plg_t&);
  void minVertexes(std::ostream&, const shirokov::plg_t&);

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

  void minArea(std::ostream& out, const shirokov::plg_t& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("AREA requires at least one polygon in the dataset");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), AreaComparator{});
    out << getPolygonArea(*it);
  }

  void minVertexes(std::ostream& out, const shirokov::plg_t& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("VERTEXES requires at least one polygon in the dataset");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), VertexComparator{});
    out << it->points.size();
  }
}

void shirokov::min(std::istream& in, std::ostream& out, plg_t& polygons)
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
  subCmds["AREA"] = minArea;
  subCmds["VERTEXES"] = minVertexes;

  if (subCmds.count(subCmd))
  {
    subCmds[subCmd](out, polygons);
  }
  else
  {
    throw std::logic_error("Unrecognized subcommand");
  }
}
