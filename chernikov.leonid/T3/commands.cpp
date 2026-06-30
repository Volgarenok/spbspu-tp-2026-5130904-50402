#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <ioguard.hpp>
#include "ioguard.hpp"
#include <map>
#include <numeric>
#include <stdexcept>
#include "geometry.hpp"
#include <iostream>
#include <iomanip>

namespace {
  bool isEven(const chernikov::Polygon &p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOdd(const chernikov::Polygon &p)
  {
    return p.points.size() % 2 != 0;
  }

  bool matchVertexes(const chernikov::Polygon &p, size_t n)
  {
    return p.points.size() == n;
  }

  template < class Filter >
  void printFilteredSum(std::ostream &out, const std::vector< chernikov::Polygon > &polygons, Filter filter)
  {
    std::vector< chernikov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), filter);
    std::vector< double > areas;
    areas.reserve(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), chernikov::calcArea);
    out << std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());
  }

  bool lessArea(const chernikov::Polygon &a, const chernikov::Polygon &b)
  {
    return chernikov::calcArea(a) < chernikov::calcArea(b);
  }

  bool lessVertex(const chernikov::Polygon &a, const chernikov::Polygon &b)
  {
    return a.points.size() < b.points.size();
  }

  void handleAreaMean(std::ostream &out, const std::vector< chernikov::Polygon > &polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("invalid");
    }
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), chernikov::calcArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());
    out << sum / polygons.size();
  }

  void handleAreaEven(std::ostream &out, const std::vector< chernikov::Polygon > &polygons)
  {
    printFilteredSum(out, polygons, isEven);
  }

  void handleAreaOdd(std::ostream &out, const std::vector< chernikov::Polygon > &polygons)
  {
    printFilteredSum(out, polygons, isOdd);
  }

  void handleMaxArea(std::ostream &out, const std::vector< chernikov::Polygon > &polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("invalid");
    }
    auto it = std::max_element(polygons.begin(), polygons.end(), lessArea);
    out << std::fixed << std::setprecision(1) << chernikov::calcArea(*it);
  }

  void handleMaxVertexes(std::ostream &out, const std::vector< chernikov::Polygon > &polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("invalid");
    }
    auto it = std::max_element(polygons.begin(), polygons.end(), lessVertex);
    out << it->points.size();
  }

  void handleMinArea(std::ostream &out, const std::vector< chernikov::Polygon > &polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("invalid");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), lessArea);
    out << std::fixed << std::setprecision(1) << chernikov::calcArea(*it);
  }

  void handleMinVertexes(std::ostream &out, const std::vector< chernikov::Polygon > &polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("invalid");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), lessVertex);
    out << it->points.size();
  }

  void handleCountEven(std::ostream &out, const std::vector< chernikov::Polygon > &polygons)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isEven);
  }

  void handleCountOdd(std::ostream &out, const std::vector< chernikov::Polygon > &polygons)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOdd);
  }
}

void chernikov::area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }

  IOguard guard(out);
  out << std::fixed << std::setprecision(1);

  std::map< std::string, void (*)(std::ostream &, const std::vector< Polygon > &) > handlers;
  handlers["MEAN"] = handleAreaMean;
  handlers["EVEN"] = handleAreaEven;
  handlers["ODD"] = handleAreaOdd;

  auto it = handlers.find(param);
  if (it != handlers.end())
  {
    it->second(out, polygons);
    return;
  }

  size_t n = std::stoul(param);
  if (n < 3)
  {
    throw std::invalid_argument("invalid");
  }
  printFilteredSum(out, polygons, std::bind(matchVertexes, std::placeholders::_1, n));
}

void chernikov::max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  IOguard guard(out);
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }

  std::map< std::string, void (*)(std::ostream &, const std::vector< Polygon > &) > handlers;
  handlers["AREA"] = handleMaxArea;
  handlers["VERTEXES"] = handleMaxVertexes;

  handlers.at(param)(out, polygons);
}

void chernikov::min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  IOguard guard(out);
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }

  std::map< std::string, void (*)(std::ostream &, const std::vector< Polygon > &) > handlers;
  handlers["AREA"] = handleMinArea;
  handlers["VERTEXES"] = handleMinVertexes;

  handlers.at(param)(out, polygons);
}

void chernikov::count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }

  std::map< std::string, void (*)(std::ostream &, const std::vector< Polygon > &) > handlers;
  handlers["EVEN"] = handleCountEven;
  handlers["ODD"] = handleCountOdd;

  auto it = handlers.find(param);
  if (it != handlers.end())
  {
    it->second(out, polygons);
    return;
  }

  size_t n = std::stoul(param);
  if (n < 3)
  {
    throw std::invalid_argument("invalid");
  }
  out << std::count_if(polygons.begin(), polygons.end(), std::bind(matchVertexes, std::placeholders::_1, n));
}

void chernikov::rects(std::istream &, std::ostream &out, const std::vector< Polygon > &polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), isRect);
}

void chernikov::rightshapes(std::istream &, std::ostream &out, const std::vector< Polygon > &polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), hasRightAngle);
}

void chernikov::perms(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  Polygon target;
  if (!(in >> target))
  {
    throw std::invalid_argument("invalid");
  }
  in >> std::ws;
  if (in.peek() != EOF)
  {
    throw std::invalid_argument("invalid");
  }

  out << std::count_if(polygons.begin(), polygons.end(), std::bind(isPermutationOf, std::placeholders::_1, target));
}

void chernikov::echo(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons)
{
  Polygon target;
  if (!(in >> target))
  {
    throw std::invalid_argument("invalid");
  }
  in >> std::ws;
  if (in.peek() != EOF)
  {
    throw std::invalid_argument("invalid");
  }

  size_t added = 0;
  std::vector< Polygon > result;

  for (const auto &p : polygons)
  {
    result.push_back(p);
    if (isPermutationOf(p, target))
    {
      result.push_back(p);
      ++added;
    }
  }

  polygons = std::move(result);
  out << added;
}
