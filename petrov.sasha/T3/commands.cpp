#include "commands.hpp"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <ios>
#include <istream>
#include <iterator>
#include <map>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <string>

#include "ioguard.hpp"

bool petrov::detail::isEven(const petrov::Polygon &p)
{
  return (p.points.size() % 2) == 0;
}

bool petrov::detail::isOdd(const petrov::Polygon &p)
{
  return (p.points.size() % 2) != 0;
}

bool petrov::detail::hasVertexCount(const petrov::Polygon &p, std::size_t n)
{
  return p.points.size() == n;
}

bool petrov::detail::isSpaceChar(char c)
{
  return c == ' ';
}

bool petrov::detail::areaLess(const petrov::Polygon &a, const petrov::Polygon &b)
{
  return calculateArea(a) < calculateArea(b);
}

bool petrov::detail::vertexLess(const petrov::Polygon &a, const petrov::Polygon &b)
{
  return a.points.size() < b.points.size();
}

petrov::detail::Bounds petrov::detail::getPolygonBounds(const petrov::Polygon &poly)
{
  const std::vector< Point > &pts = poly.points;
  int minX = std::min_element(pts.begin(), pts.end(), compareByX)->x;
  int maxX = std::max_element(pts.begin(), pts.end(), compareByX)->x;
  int minY = std::min_element(pts.begin(), pts.end(), compareByY)->y;
  int maxY = std::max_element(pts.begin(), pts.end(), compareByY)->y;
  return {minX, minY, maxX, maxY};
}

bool petrov::detail::compareByMinX(const petrov::detail::Bounds &a, const petrov::detail::Bounds &b)
{
  return a.minX < b.minX;
}

bool petrov::detail::compareByMinY(const petrov::detail::Bounds &a, const petrov::detail::Bounds &b)
{
  return a.minY < b.minY;
}

bool petrov::detail::compareByMaxX(const petrov::detail::Bounds &a, const petrov::detail::Bounds &b)
{
  return a.maxX < b.maxX;
}

bool petrov::detail::compareByMaxY(const petrov::detail::Bounds &a, const petrov::detail::Bounds &b)
{
  return a.maxY < b.maxY;
}

bool petrov::detail::isPointInsideFrame(const petrov::Point &p, int minX, int minY, int maxX, int maxY)
{
  return p.x >= minX && p.x <= maxX && p.y >= minY && p.y <= maxY;
}

std::size_t petrov::detail::convertVertexCount(const std::string &param)
{
  std::size_t pos = 0;
  std::size_t count = std::stoul(param, &pos);
  if (pos != param.size() || count < 3) {
    throw std::invalid_argument("invalid");
  }
  return count;
}

petrov::Polygon petrov::detail::readPolygonParam(std::istream &in)
{
  Polygon target;
  in >> target;
  if (!in || target.points.empty()) {
    throw std::invalid_argument("invalid");
  }

  std::string rest;
  std::getline(in, rest);
  if (!std::all_of(rest.begin(), rest.end(), isSpaceChar)) {
    throw std::invalid_argument("invalid");
  }

  return target;
}

void petrov::detail::printFilteredSum(
  std::ostream &out,
  const std::vector< petrov::Polygon > &polygons,
  std::function< bool(const petrov::Polygon &) > pred
)
{
  IOGuard guard(out);
  std::vector< Polygon > filtered;
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), pred);
  std::vector< double > areas;
  areas.reserve(filtered.size());
  std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), calculateArea);
  double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
  out << std::fixed << std::setprecision(1) << sum;
}

void petrov::detail::printMeanArea(std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  if (polygons.empty()) {
    throw std::invalid_argument("invalid");
  }
  IOGuard guard(out);
  std::vector< double > areas;
  areas.reserve(polygons.size());
  std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), calculateArea);
  double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
  out << std::fixed << std::setprecision(1) << sum / polygons.size();
}

void petrov::detail::printMaxArea(std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  IOGuard guard(out);
  auto it = std::max_element(polygons.begin(), polygons.end(), areaLess);
  out << std::fixed << std::setprecision(1) << calculateArea(*it);
}

void petrov::detail::printMaxVertexes(std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  auto it = std::max_element(polygons.begin(), polygons.end(), vertexLess);
  out << it->points.size();
}

void petrov::detail::printMinArea(std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  IOGuard guard(out);
  auto it = std::min_element(polygons.begin(), polygons.end(), areaLess);
  out << std::fixed << std::setprecision(1) << calculateArea(*it);
}

void petrov::detail::printMinVertexes(std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  auto it = std::min_element(polygons.begin(), polygons.end(), vertexLess);
  out << it->points.size();
}

void petrov::detail::printEvenCount(std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), isEven);
}

void petrov::detail::printOddCount(std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), isOdd);
}

void petrov::detail::printVertexCount(
  std::ostream &out,
  const std::vector< petrov::Polygon > &polygons,
  std::size_t count
)
{
  out << std::count_if(
    polygons.begin(),
    polygons.end(),
    std::bind(hasVertexCount, std::placeholders::_1, count)
  );
}

void petrov::area(std::istream &in, std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  using command_t = std::function< void() >;
  std::map< std::string, command_t > subcommands;
  subcommands["MEAN"] = std::bind(detail::printMeanArea, std::ref(out), std::cref(polygons));
  subcommands["EVEN"] = std::bind(
    detail::printFilteredSum,
    std::ref(out),
    std::cref(polygons),
    std::function< bool(const Polygon &) >(detail::isEven)
  );
  subcommands["ODD"] = std::bind(
    detail::printFilteredSum,
    std::ref(out),
    std::cref(polygons),
    std::function< bool(const Polygon &) >(detail::isOdd)
  );

  auto it = subcommands.find(param);
  if (it != subcommands.end()) {
    it->second();
    return;
  }

  std::size_t count = detail::convertVertexCount(param);
  detail::printFilteredSum(
    out,
    polygons,
    std::bind(detail::hasVertexCount, std::placeholders::_1, count)
  );
}

void petrov::max(std::istream &in, std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  std::string param;
  if (!(in >> param) || polygons.empty()) {
    throw std::invalid_argument("invalid");
  }

  using command_t = std::function< void() >;
  std::map< std::string, command_t > subcommands;
  subcommands["AREA"] = std::bind(detail::printMaxArea, std::ref(out), std::cref(polygons));
  subcommands["VERTEXES"] = std::bind(detail::printMaxVertexes, std::ref(out), std::cref(polygons));

  subcommands.at(param)();
}

void petrov::min(std::istream &in, std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  std::string param;
  if (!(in >> param) || polygons.empty()) {
    throw std::invalid_argument("invalid");
  }

  using command_t = std::function< void() >;
  std::map< std::string, command_t > subcommands;
  subcommands["AREA"] = std::bind(detail::printMinArea, std::ref(out), std::cref(polygons));
  subcommands["VERTEXES"] = std::bind(detail::printMinVertexes, std::ref(out), std::cref(polygons));

  subcommands.at(param)();
}

void petrov::count(std::istream &in, std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  using command_t = std::function< void() >;
  std::map< std::string, command_t > subcommands;
  subcommands["EVEN"] = std::bind(detail::printEvenCount, std::ref(out), std::cref(polygons));
  subcommands["ODD"] = std::bind(detail::printOddCount, std::ref(out), std::cref(polygons));

  auto it = subcommands.find(param);
  if (it != subcommands.end()) {
    it->second();
    return;
  }

  std::size_t vertexCount = detail::convertVertexCount(param);
  detail::printVertexCount(out, polygons, vertexCount);
}

void petrov::perms(std::istream &in, std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  Polygon target = detail::readPolygonParam(in);
  Polygon swappedTarget = swapCoordinates(target);
  auto direct = std::bind(isPermutationOf, std::placeholders::_1, std::cref(target));
  auto swapped = std::bind(isPermutationOf, std::placeholders::_1, std::cref(swappedTarget));
  auto pred = std::bind(std::logical_or< bool >(), direct, swapped);
  out << std::count_if(polygons.begin(), polygons.end(), pred);
}

void petrov::inframe(std::istream &in, std::ostream &out, const std::vector< petrov::Polygon > &polygons)
{
  Polygon target = detail::readPolygonParam(in);

  if (polygons.empty()) {
    out << "<FALSE>";
    return;
  }

  std::vector< detail::Bounds > bounds(polygons.size());
  std::transform(polygons.begin(), polygons.end(), bounds.begin(), detail::getPolygonBounds);

  int globalMinX = std::min_element(bounds.begin(), bounds.end(), detail::compareByMinX)->minX;
  int globalMinY = std::min_element(bounds.begin(), bounds.end(), detail::compareByMinY)->minY;
  int globalMaxX = std::max_element(bounds.begin(), bounds.end(), detail::compareByMaxX)->maxX;
  int globalMaxY = std::max_element(bounds.begin(), bounds.end(), detail::compareByMaxY)->maxY;

  auto pred = std::bind(
    detail::isPointInsideFrame,
    std::placeholders::_1,
    globalMinX,
    globalMinY,
    globalMaxX,
    globalMaxY
  );
  bool inside = std::all_of(target.points.begin(), target.points.end(), pred);
  out << (inside ? "<TRUE>" : "<FALSE>");
}
