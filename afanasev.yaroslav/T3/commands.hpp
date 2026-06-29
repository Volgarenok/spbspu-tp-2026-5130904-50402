#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <limits>
#include <numeric>
#include <cctype>
#include <unordered_map>
#include "shape.hpp"

namespace afanasev
{
  void area(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void max(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void min(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void count(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void perms(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
  void rects(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
}

namespace
{
  using namespace afanasev;

  bool isEven(const Polygon & p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOdd(const Polygon & p)
  {
    return p.points.size() % 2 != 0;
  }

  bool hasVertexCount(const Polygon & p, size_t n)
  {
    return p.points.size() == n;
  }

  template < class Pred >
  void printFilteredSum(std::ostream & out, const std::vector< Polygon > & polygons, Pred pred)
  {
    IOguard guard(out);
    std::vector< Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), pred);
    std::vector< double > areas;
    areas.reserve(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), makeArea);
    out << std::fixed << std::setprecision(1)
      << std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  bool areaLess(const Polygon & a, const Polygon & b)
  {
    return makeArea(a) < makeArea(b);
  }

  bool vertexLess(const Polygon & a, const Polygon & b)
  {
    return a.points.size() < b.points.size();
  }

  bool readPolygon(std::istream & in, Polygon & p)
  {
    size_t n;
    if (!(in >> n))
    {
      return false;
    }
    if (n < 3)
    {
      return false;
    }

    std::vector< Point > pts;
    pts.reserve(n);
    for (size_t i = 0; i < n; ++i)
    {
      char open, comma, close;
      int x, y;
      if (!(in >> open >> x >> comma >> y >> close) ||
          open != '(' || comma != ';' || close != ')')
        return false;
      pts.push_back({x, y});
    }

    p.points = std::move(pts);
    return true;
  }

  bool isSpaceChar(char c)
  {
    return c == ' ';
  }
}

void afanasev::area(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }

  if (param == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("invalid");
    }
    IOguard guard(out);
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), makeArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << sum / polygons.size();
  }

  else if (param == "EVEN")
  {
    printFilteredSum(out, polygons, isEven);
  }
  else if (param == "ODD")
  {
    printFilteredSum(out, polygons, isOdd);
  }
  else
  {
    size_t n = std::stoul(param);
    if (n < 3)
    {
      throw std::invalid_argument("invalid");
    }
    printFilteredSum(out, polygons, std::bind(hasVertexCount, std::placeholders::_1, n));
  }
}

void afanasev::max(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }
  if (polygons.empty())
  {
    throw std::invalid_argument("invalid");
  }

  if (param == "AREA")
  {
    IOguard guard(out);
    auto it = std::max_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << makeArea(*it);
  }
  else if (param == "VERTEXES")
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), vertexLess);
    out << it->points.size();
  }
  else
  {
    throw std::invalid_argument("invalid");
  }
}

void afanasev::min(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }
  if (polygons.empty())
  {
    throw std::invalid_argument("invalid");
  }

  if (param == "AREA")
  {
    IOguard guard(out);
    auto it = std::min_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << makeArea(*it);
  }
  else if (param == "VERTEXES")
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), vertexLess);
    out << it->points.size();
  }
  else
  {
    throw std::invalid_argument("invalid");
  }
}

void afanasev::count(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }

  if (param == "EVEN")
  {
    out << std::count_if(polygons.begin(), polygons.end(), isEven);
  }
  else if (param == "ODD")
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOdd);
  }
  else
  {
    size_t n = std::stoul(param);
    if (n < 3)
    {
      throw std::invalid_argument("invalid");
    }
    out << std::count_if(polygons.begin(), polygons.end(),
      std::bind(hasVertexCount, std::placeholders::_1, n));
  }
}

void afanasev::perms(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  Polygon target;
  if (!readPolygon(in, target))
  {
    throw std::invalid_argument("invalid");
  }

  std::string rest;
  std::getline(in, rest);
  if (!std::all_of(rest.begin(), rest.end(), isSpaceChar))
  {
    throw std::invalid_argument("invalid");
  }

  Polygon swapped = swapCoordinates(target);
  auto isPerm = [&target, &swapped](const Polygon & p) -> bool
  {
    return isPermutationOf(p, target) || isPermutationOf(p, swapped);
  };
  out << std::count_if(polygons.begin(), polygons.end(), isPerm);
}

void afanasev::rects(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::string rest;
  std::getline(in, rest);
  if (!std::all_of(rest.begin(), rest.end(), isSpaceChar))
  {
    throw std::invalid_argument("invalid");
  }

  out << std::count_if(polygons.begin(), polygons.end(), isRectangle);
}

#endif
