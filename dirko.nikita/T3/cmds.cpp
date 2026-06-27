#include "cmds.hpp"
#include <algorithm>
#include <functional>
#include <ioguard.hpp>
#include <iomanip>
#include <map>
#include <numeric>
#include <stdexcept>
#include "polygon.hpp"

namespace
{
  bool isEven(const dirko::Polygon &p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOdd(const dirko::Polygon &p)
  {
    return p.points.size() % 2 != 0;
  }
  bool matchVertexies(const dirko::Polygon &p, size_t n)
  {
    return p.points.size() == n;
  }
  template< class Filter >
  void printFilteredSum(std::ostream &out, const std::vector< dirko::Polygon > &polygons, Filter filter)
  {
    std::vector< dirko::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), filter);
    std::vector< double > areas;
    areas.reserve(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), dirko::calcArea);
    out << std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());
  }
  bool lessArea(const dirko::Polygon &a, const dirko::Polygon &b)
  {
    return dirko::calcArea(a) < dirko::calcArea(b);
  }

  bool lessVertex(const dirko::Polygon &a, const dirko::Polygon &b)
  {
    return a.points.size() < b.points.size();
  }
  bool lessCoord(const dirko::Point &a, const dirko::Point &b, int dirko::Point::*coord)
  {
    return a.*coord < b.*coord;
  }
  int getMinX(const dirko::Polygon &poly)
  {
    using namespace std::placeholders;
    return std::min_element(poly.points.begin(), poly.points.end(), std::bind(lessCoord, _1, _2, &dirko::Point::x))->x;
  }
  int getMaxX(const dirko::Polygon &poly)
  {
    using namespace std::placeholders;
    return std::max_element(poly.points.begin(), poly.points.end(), std::bind(lessCoord, _1, _2, &dirko::Point::x))->x;
  }
  int getMinY(const dirko::Polygon &poly)
  {
    using namespace std::placeholders;
    return std::min_element(poly.points.begin(), poly.points.end(), std::bind(lessCoord, _1, _2, &dirko::Point::y))->y;
  }
  int getMaxY(const dirko::Polygon &poly)
  {
    using namespace std::placeholders;
    return std::max_element(poly.points.begin(), poly.points.end(), std::bind(lessCoord, _1, _2, &dirko::Point::y))->y;
  }
  bool pointInFrame(const dirko::Point &p, int minX, int maxX, int minY, int maxY)
  {
    return p.x >= minX && p.x <= maxX && p.y >= minY && p.y <= maxY;
  }

  void handleAreaMean(std::ostream &out, const std::vector< dirko::Polygon > &polygons)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), dirko::calcArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());
    out << sum / polygons.size();
  }

  void handleAreaEven(std::ostream &out, const std::vector< dirko::Polygon > &polygons)
  {
    printFilteredSum(out, polygons, isEven);
  }

  void handleAreaOdd(std::ostream &out, const std::vector< dirko::Polygon > &polygons)
  {
    printFilteredSum(out, polygons, isOdd);
  }

  void handleMaxArea(std::ostream &out, const std::vector< dirko::Polygon > &polygons)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    auto it = std::max_element(polygons.begin(), polygons.end(), lessArea);
    out << std::fixed << std::setprecision(1) << dirko::calcArea(*it);
  }

  void handleMaxVertexes(std::ostream &out, const std::vector< dirko::Polygon > &polygons)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    auto it = std::max_element(polygons.begin(), polygons.end(), lessVertex);
    out << it->points.size();
  }

  void handleMinArea(std::ostream &out, const std::vector< dirko::Polygon > &polygons)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), lessArea);
    out << std::fixed << std::setprecision(1) << dirko::calcArea(*it);
  }

  void handleMinVertexes(std::ostream &out, const std::vector< dirko::Polygon > &polygons)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), lessVertex);
    out << it->points.size();
  }

  void handleCountEven(std::ostream &out, const std::vector< dirko::Polygon > &polygons)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isEven);
  }

  void handleCountOdd(std::ostream &out, const std::vector< dirko::Polygon > &polygons)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOdd);
  }
}

void dirko::area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  IOguard guard(in);
  out << std::fixed << std::setprecision(1);

  std::map< std::string, void (*)(std::ostream &, const std::vector< Polygon > &) > handlers;
  handlers["MEAN"] = handleAreaMean;
  handlers["EVEN"] = handleAreaEven;
  handlers["ODD"] = handleAreaOdd;

  auto it = handlers.find(param);
  if (it != handlers.end()) {
    it->second(out, polygons);
    return;
  }

  size_t n = std::stoul(param);
  if (n < 3) {
    throw std::invalid_argument("invalid");
  }
  printFilteredSum(out, polygons, std::bind(matchVertexies, std::placeholders::_1, n));
}

void dirko::max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  IOguard guard(in);
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }
  std::map< std::string, void (*)(std::ostream &, const std::vector< Polygon > &) > handlers;
  handlers["AREA"] = handleMaxArea;
  handlers["VERTEXES"] = handleMaxVertexes;

  handlers.at(param)(out, polygons);
}

void dirko::min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  IOguard guard(in);
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }
  std::map< std::string, void (*)(std::ostream &, const std::vector< Polygon > &) > handlers;
  handlers["AREA"] = handleMinArea;
  handlers["VERTEXES"] = handleMinVertexes;

  handlers.at(param)(out, polygons);
}

void dirko::count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  std::map< std::string, void (*)(std::ostream &, const std::vector< Polygon > &) > handlers;
  handlers["EVEN"] = handleCountEven;
  handlers["ODD"] = handleCountOdd;

  auto it = handlers.find(param);
  if (it != handlers.end()) {
    it->second(out, polygons);
    return;
  }
  size_t n = std::stoul(param);
  if (n < 3) {
    throw std::invalid_argument("invalid");
  }
  out << std::count_if(polygons.begin(), polygons.end(), std::bind(matchVertexies, std::placeholders::_1, n));
}
void dirko::rects(std::istream &, std::ostream &out, const std::vector< Polygon > &polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), isRect);
}

void dirko::inframe(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  if (polygons.empty()) {
    throw std::invalid_argument("invalid");
  }
  Polygon pol;
  if (!(in >> pol)) {
    throw std::invalid_argument("invalid");
  }
  in >> std::ws;
  if (in.peek() != EOF || in.peek() != '\n') {
    throw std::invalid_argument("invalid");
  }

  std::vector< int > minX_list(polygons.size());
  std::vector< int > maxX_list(polygons.size());
  std::vector< int > minY_list(polygons.size());
  std::vector< int > maxY_list(polygons.size());

  std::transform(polygons.begin(), polygons.end(), minX_list.begin(), getMinX);
  std::transform(polygons.begin(), polygons.end(), maxX_list.begin(), getMaxX);
  std::transform(polygons.begin(), polygons.end(), minY_list.begin(), getMinY);
  std::transform(polygons.begin(), polygons.end(), maxY_list.begin(), getMaxY);

  int MinX = *std::min_element(minX_list.begin(), minX_list.end());
  int MaxX = *std::max_element(maxX_list.begin(), maxX_list.end());
  int MinY = *std::min_element(minY_list.begin(), minY_list.end());
  int MaxY = *std::max_element(maxY_list.begin(), maxY_list.end());

  auto func = std::bind(pointInFrame, std::placeholders::_1, MinX, MaxX, MinY, MaxY);
  out << (std::all_of(pol.points.begin(), pol.points.end(), func) ? "<TRUE>" : "<FALSE>");
}
