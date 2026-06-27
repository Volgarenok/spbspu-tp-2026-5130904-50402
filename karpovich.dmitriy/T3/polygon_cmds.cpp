#include "polygon_cmds.hpp"
#include <algorithm>
#include <functional>
#include <ioguard.hpp>
#include <iomanip>
#include <limits>
#include <numeric>

namespace
{

  double getArea(const karpovich::Polygon &p)
  {
    return karpovich::calculateArea(p);
  }

  bool isEven(const karpovich::Polygon &p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOdd(const karpovich::Polygon &p)
  {
    return p.points.size() % 2 != 0;
  }

  bool hasVertexCount(const karpovich::Polygon &p, size_t n)
  {
    return p.points.size() == n;
  }

  bool areaLess(const karpovich::Polygon &a, const karpovich::Polygon &b)
  {
    return karpovich::calculateArea(a) < karpovich::calculateArea(b);
  }

  bool vertexLess(const karpovich::Polygon &a, const karpovich::Polygon &b)
  {
    return a.points.size() < b.points.size();
  }

  template< class Pred >
  void printFilteredSum(std::ostream &out, const std::vector< karpovich::Polygon > &polygons, Pred pred)
  {
    std::vector< karpovich::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), pred);
    std::vector< double > areas;
    areas.reserve(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), getArea);
    out << std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());
  }

  bool isSpaceChar(char c)
  {
    return c == ' ';
  }
}

void karpovich::area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  IOguard guard(in);
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  out << std::fixed << std::setprecision(1);

  if (param == "MEAN") {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());
    out << sum / polygons.size() << "\n";
  } else if (param == "EVEN") {
    printFilteredSum(out, polygons, isEven);
    out << '\n';
  } else if (param == "ODD") {
    printFilteredSum(out, polygons, isOdd);
    out << '\n';
  } else {
    size_t n = std::stoul(param);
    if (n < 3) {
      throw std::invalid_argument("invalid");
    }
    printFilteredSum(out, polygons, std::bind(hasVertexCount, std::placeholders::_1, n));
    out << '\n';
  }
}

void karpovich::max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  IOguard guard(in);
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }
  if (polygons.empty()) {
    throw std::invalid_argument("invalid");
  }

  if (param == "AREA") {
    auto it = std::max_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << karpovich::calculateArea(*it) << "\n";
  } else if (param == "VERTEXES") {
    auto it = std::max_element(polygons.begin(), polygons.end(), vertexLess);
    out << it->points.size() << "\n";
  } else {
    throw std::invalid_argument("invalid");
  }
}

void karpovich::min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  IOguard guard(in);
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }
  if (polygons.empty()) {
    throw std::invalid_argument("invalid");
  }

  if (param == "AREA") {
    auto it = std::min_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << karpovich::calculateArea(*it) << "\n";
  } else if (param == "VERTEXES") {
    auto it = std::min_element(polygons.begin(), polygons.end(), vertexLess);
    out << it->points.size() << "\n";
  } else {
    throw std::invalid_argument("invalid");
  }
}

void karpovich::count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  if (param == "EVEN") {
    out << std::count_if(polygons.begin(), polygons.end(), isEven) << "\n";
  } else if (param == "ODD") {
    out << std::count_if(polygons.begin(), polygons.end(), isOdd) << "\n";
  } else {
    size_t n = std::stoul(param);
    if (n < 3) {
      throw std::invalid_argument("invalid");
    }
    auto pred = std::bind(hasVertexCount, std::placeholders::_1, n);
    out << std::count_if(polygons.begin(), polygons.end(), pred) << "\n";
  }
}

void karpovich::rightshapes(std::istream &, std::ostream &out, const std::vector< Polygon > &polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), &karpovich::hasRightAngle) << "\n";
}

void karpovich::same(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  if (polygons.empty()) {
    throw std::invalid_argument("invalid");
  }
  Polygon target;
  if (!(in >> target)) {
    throw std::invalid_argument("invalid");
  }
  if (target.points.size() < 3) {
    throw std::invalid_argument("invalid");
  }
  std::string rest;
  std::getline(in, rest);
  if (!std::all_of(rest.begin(), rest.end(), isSpaceChar)) {
    throw std::invalid_argument("invalid");
  }
  auto func = std::bind(&karpovich::isSame, std::placeholders::_1, std::cref(target));
  out << std::count_if(polygons.begin(), polygons.end(), func) << "\n";
}
