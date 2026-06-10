#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <numeric>
#include <stdexcept>
#include "polygon.hpp"

namespace
{
  bool isEven(const saldaev::Polygon &p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOdd(const saldaev::Polygon &p)
  {
    return p.points.size() % 2 != 0;
  }

  bool matchVertexies(const saldaev::Polygon &p, size_t n)
  {
    return p.points.size() == n;
  }

  template< class Filter >
  void printFilteredSum(std::ostream &out, const std::vector< saldaev::Polygon > &polygons, Filter filter)
  {
    std::vector< saldaev::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), filter);
    std::vector< double > areas;
    areas.reserve(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), saldaev::calcArea);
    out << std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());
  }

  bool lessArea(const saldaev::Polygon &a, const saldaev::Polygon &b)
  {
    return saldaev::calcArea(a) < saldaev::calcArea(b);
  }

  bool lessVertex(const saldaev::Polygon &a, const saldaev::Polygon &b)
  {
    return a.points.size() < b.points.size();
  }
}

void saldaev::handleArea(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
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
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), saldaev::calcArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());
    out << sum / polygons.size();
  } else if (param == "EVEN") {
    printFilteredSum(out, polygons, isEven);
  } else if (param == "ODD") {
    printFilteredSum(out, polygons, isOdd);
  } else {
    size_t n = std::stoul(param);
    if (n < 3) {
      throw std::invalid_argument("invalid");
    }
    printFilteredSum(out, polygons, std::bind(matchVertexies, std::placeholders::_1, n));
  }
}

void saldaev::handleMax(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  if (polygons.empty()) {
    throw std::invalid_argument("invalid");
  }
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  if (param == "AREA") {
    auto it = std::max_element(polygons.begin(), polygons.end(), lessArea);
    out << std::fixed << std::setprecision(1) << saldaev::calcArea(*it);
  } else if (param == "VERTEXES") {
    auto it = std::max_element(polygons.begin(), polygons.end(), lessVertex);
    out << it->points.size();
  } else {
    throw std::invalid_argument("invalid");
  }
}

void saldaev::handleMin(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  if (polygons.empty()) {
    throw std::invalid_argument("invalid");
  }
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  if (param == "AREA") {
    auto it = std::min_element(polygons.begin(), polygons.end(), lessArea);
    out << std::fixed << std::setprecision(1) << calcArea(*it);
  } else if (param == "VERTEXES") {
    auto it = std::min_element(polygons.begin(), polygons.end(), lessVertex);
    out << it->points.size();
  } else {
    throw std::invalid_argument("invalid");
  }
}

void saldaev::handleCount(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  if (param == "EVEN") {
    out << std::count_if(polygons.begin(), polygons.end(), isEven);
  } else if (param == "ODD") {
    out << std::count_if(polygons.begin(), polygons.end(), isOdd);
  } else {
    size_t n = std::stoul(param);
    if (n < 3) {
      throw std::invalid_argument("invalid");
    }
    out << std::count_if(polygons.begin(), polygons.end(), std::bind(matchVertexies, std::placeholders::_1, n));
  }
}
