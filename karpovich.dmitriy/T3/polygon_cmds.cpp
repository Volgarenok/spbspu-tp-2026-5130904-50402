#include "polygon_cmds.hpp"
#include <algorithm>
#include <functional>
#include <ioguard.hpp>
#include <iomanip>
#include <map>
#include <numeric>
#include <string>
#include <vector>
#include "shapes.hpp"

namespace
{
  using vecp_t = std::vector< karpovich::Polygon >;
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
  void printFilteredSum(std::ostream &out, const vecp_t &polygons, Pred pred)
  {
    vecp_t filtered;
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

  void handleAreaMean(std::ostream &out, const vecp_t &polygons)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());
    out << sum / polygons.size() << "\n";
  }

  void handleAreaEven(std::ostream &out, const vecp_t &polygons)
  {
    printFilteredSum(out, polygons, isEven);
    out << '\n';
  }

  void handleAreaOdd(std::ostream &out, const vecp_t &polygons)
  {
    printFilteredSum(out, polygons, isOdd);
    out << '\n';
  }

  void handleMaxArea(std::ostream &out, const vecp_t &polygons)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    auto it = std::max_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << karpovich::calculateArea(*it) << "\n";
  }

  void handleMaxVertexes(std::ostream &out, const vecp_t &polygons)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    auto it = std::max_element(polygons.begin(), polygons.end(), vertexLess);
    out << it->points.size() << "\n";
  }

  void handleMinArea(std::ostream &out, const vecp_t &polygons)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << karpovich::calculateArea(*it) << "\n";
  }

  void handleMinVertexes(std::ostream &out, const vecp_t &polygons)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), vertexLess);
    out << it->points.size() << "\n";
  }

  void handleCountEven(std::ostream &out, const vecp_t &polygons)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isEven) << "\n";
  }

  void handleCountOdd(std::ostream &out, const vecp_t &polygons)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOdd) << "\n";
  }

  void handleContextMinArea(std::ostream &out, vecp_t &polygons, std::vector< vecp_t > &contextStack)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), areaLess);
    double minArea = karpovich::calculateArea(*it);
    vecp_t newContext;
    auto pred = std::bind(std::equal_to< double >(), std::bind(getArea, std::placeholders::_1), minArea);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(newContext), pred);
    if (newContext.empty()) {
      out << "<EMPTY CONTEXT>\n";
      return;
    }
    contextStack.push_back(polygons);
    polygons = newContext;
  }

  void handleContextMaxArea(std::ostream &out, vecp_t &polygons, std::vector< vecp_t > &contextStack)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("invalid");
    }
    auto it = std::max_element(polygons.begin(), polygons.end(), areaLess);
    double maxArea = karpovich::calculateArea(*it);
    vecp_t newContext;
    auto pred = std::bind(std::equal_to< double >(), std::bind(getArea, std::placeholders::_1), maxArea);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(newContext), pred);
    if (newContext.empty()) {
      out << "<EMPTY CONTEXT>\n";
      return;
    }
    contextStack.push_back(polygons);
    polygons = newContext;
  }

  void handleContextEven(std::ostream &out, vecp_t &polygons, std::vector< vecp_t > &contextStack)
  {
    vecp_t newContext;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(newContext), isEven);
    if (newContext.empty()) {
      out << "<EMPTY CONTEXT>\n";
      return;
    }
    contextStack.push_back(polygons);
    polygons = newContext;
  }

  void handleContextOdd(std::ostream &out, vecp_t &polygons, std::vector< vecp_t > &contextStack)
  {
    vecp_t newContext;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(newContext), isOdd);
    if (newContext.empty()) {
      out << "<EMPTY CONTEXT>\n";
      return;
    }
    contextStack.push_back(polygons);
    polygons = newContext;
  }

  void handleContextVertex(std::ostream &out, vecp_t &polygons, std::vector< vecp_t > &contextStack, size_t n)
  {
    vecp_t newContext;
    auto pred = std::bind(hasVertexCount, std::placeholders::_1, n);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(newContext), pred);
    if (newContext.empty()) {
      out << "<EMPTY CONTEXT>\n";
      return;
    }
    contextStack.push_back(polygons);
    polygons = newContext;
  }
}

void karpovich::area(std::istream &in, std::ostream &out, const vecp_t &polygons)
{
  IOguard guard(in);
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  out << std::fixed << std::setprecision(1);

  std::map< std::string, void (*)(std::ostream &, const vecp_t &) > handlers;
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
  printFilteredSum(out, polygons, std::bind(hasVertexCount, std::placeholders::_1, n));
  out << '\n';
}

void karpovich::max(std::istream &in, std::ostream &out, const vecp_t &polygons)
{
  IOguard guard(in);
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }
  std::map< std::string, void (*)(std::ostream &, const vecp_t &) > handlers;
  handlers["AREA"] = handleMaxArea;
  handlers["VERTEXES"] = handleMaxVertexes;
  auto it = handlers.find(param);
  if (it == handlers.end()) {
    throw std::invalid_argument("invalid");
  }
  it->second(out, polygons);
}

void karpovich::min(std::istream &in, std::ostream &out, const vecp_t &polygons)
{
  IOguard guard(in);
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }
  std::map< std::string, void (*)(std::ostream &, const vecp_t &) > handlers;
  handlers["AREA"] = handleMinArea;
  handlers["VERTEXES"] = handleMinVertexes;

  auto it = handlers.find(param);
  if (it == handlers.end()) {
    throw std::invalid_argument("invalid");
  }
  it->second(out, polygons);
}

void karpovich::count(std::istream &in, std::ostream &out, const vecp_t &polygons)
{
  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }

  std::map< std::string, void (*)(std::ostream &, const vecp_t &) > handlers;
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
  out << std::count_if(polygons.begin(), polygons.end(), std::bind(hasVertexCount, std::placeholders::_1, n)) << "\n";
}

void karpovich::rightshapes(std::istream &, std::ostream &out, const vecp_t &polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), &karpovich::hasRightAngle) << "\n";
}

void karpovich::same(std::istream &in, std::ostream &out, const vecp_t &polygons)
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

void karpovich::context(std::istream &in, std::ostream &out, vecp_t &polygons, std::vector< vecp_t > &contextStack)
{
  if (polygons.empty()) {
    throw std::invalid_argument("invalid");
  }

  std::string param;
  if (!(in >> param)) {
    throw std::invalid_argument("invalid");
  }
  std::map< std::string, void (*)(std::ostream &, vecp_t &, std::vector< vecp_t > &) > handlers;
  handlers["MIN-AREA"] = handleContextMinArea;
  handlers["MAX-AREA"] = handleContextMaxArea;
  handlers["EVEN"] = handleContextEven;
  handlers["ODD"] = handleContextOdd;

  auto it = handlers.find(param);
  if (it != handlers.end()) {
    it->second(out, polygons, contextStack);
    return;
  }

  size_t n = std::stoul(param);
  if (n < 3) {
    throw std::invalid_argument("invalid");
  }
  handleContextVertex(out, polygons, contextStack, n);
}

void karpovich::popcontext(std::istream &, std::ostream &out, std::vector< vecp_t > &contextStack)
{
  if (contextStack.empty()) {
    out << "<COMMON CONTEXT>\n";
    return;
  }
  contextStack.pop_back();
}

void karpovich::level(std::istream &, std::ostream &out, const std::vector< vecp_t > &contextStack)
{
  out << "<LEVEL: " << contextStack.size() << ">\n";
}
