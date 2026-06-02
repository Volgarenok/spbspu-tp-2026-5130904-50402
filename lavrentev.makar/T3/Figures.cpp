#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <cmath>
#include <algorithm>
#include "Figures.hpp"

using namespace std::placeholders;

void lavrentev::area(std::istream& is, std::vector< Polygon > plgs)
{
  std::map< std::string, void (*)(const std::vector< Polygon >&) > cmds;
  cmds["EVEN"] = areaEven;
  cmds["ODD"] = areaOdd;
  cmds["MEAN"] = areaMean;
  std::string param;
  is >> param;
  if (cmds.find(param) != cmds.end())
  {
    cmds[param](plgs);
    return;
  }
  try
  {
    if (param[0] == '-') {
      throw std::invalid_argument("Negative number");
    }
    size_t pos;

    unsigned long long result = std::stoull(param, &pos);
    if (pos == param.length()) {
      size_t n = static_cast<size_t>(result);
      areaVrtxs(plgs, n);
    } else {
      throw std::invalid_argument("Invalid number");
    }
  }
  catch (...)
  {
    throw std::invalid_argument("Invalid number");
  }
}

void lavrentev::areaEven(const std::vector< Polygon >& plgs)
{
  std::vector< Polygon > evenPlgs;
  std::copy_if(plgs.begin(), plgs.end(), std::back_inserter(evenPlgs), isEven);
  std::vector< float > areas(evenPlgs.size());
  std::transform(
    evenPlgs.begin(),
    evenPlgs.end(),
    areas.begin(),
    std::bind(&lavrentev::Polygon::getArea, _1)
  );
  float total = std::accumulate(areas.begin(), areas.end(), 0.0f);
  std::cout << total << "\n";
}

bool lavrentev::isEven(Polygon p)
{
  return p.points.size() % 2 == 0;
}

bool lavrentev::isOdd(Polygon p)
{
  return !isEven(p);
}

void lavrentev::areaOdd(const std::vector< Polygon >& plgs)
{
  std::vector< Polygon > oddPlgs;
  std::copy_if(plgs.begin(), plgs.end(), std::back_inserter(oddPlgs), isOdd);
  std::vector< float > areas(oddPlgs.size());
  std::transform(
    oddPlgs.begin(),
    oddPlgs.end(),
    areas.begin(),
    std::bind(&lavrentev::Polygon::getArea, _1)
  );
  float total = std::accumulate(areas.begin(), areas.end(), 0.0f);
  std::cout << total << "\n";
}

const float lavrentev::Polygon::getArea() const
{
  std::vector< Triangle > triangles(points.size() - 2);
  size_t index = 1;
  std::generate(
    triangles.begin(), 
    triangles.end(), 
    std::bind(&lavrentev::Polygon::helpArea, std::ref(points), std::ref(index))
  );

  std::vector< float > areas(triangles.size());
  std::transform(
    triangles.begin(),
    triangles.end(),
    areas.begin(),
    std::bind(&lavrentev::Triangle::getArea, _1)
  );
  float total = std::accumulate(areas.begin(), areas.end(), 0.0f);

  return total;
}

lavrentev::Triangle lavrentev::Polygon::helpArea(const std::vector< Point >& points, size_t& index)
{
  Triangle ans = Triangle(points[0], points[index], points[index + 1]);
  ++index;
  return ans;
}

const float lavrentev::Triangle::getArea() const
{
  int x1 = points[0].x;
  int x2 = points[1].x;
  int x3 = points[2].x;
  int y1 = points[0].y;
  int y2 = points[1].y;
  int y3 = points[2].y;

  return 0.5 * std::abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
}


