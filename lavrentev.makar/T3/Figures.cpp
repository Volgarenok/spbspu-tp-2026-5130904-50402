#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <cmath>
#include <algorithm>
#include "Figures.hpp"

using namespace std::placeholders;

void lavrentev::area(std::istream& is, const std::vector< Polygon > plgs)
{
  std::map< std::string, void (*)(std::istream&, const std::vector< Polygon >&) > cmds;
  cmds["EVEN"] = areaEven;
  cmds["ODD"] = areaOdd;
  cmds["MEAN"] = areaMean;
  std::string param;
  is >> param;
  if (cmds.find(param) != cmds.end())
  {
    cmds[param](is, plgs);
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
    throw std::invalid_argument("Invalid command");
  }
}

void lavrentev::areaEven(std::istream&, const std::vector< Polygon >& plgs)
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
  return p.getSize() % 2 == 0;
}

bool lavrentev::isOdd(Polygon p)
{
  return !isEven(p);
}

void lavrentev::areaOdd(std::istream&, const std::vector< Polygon >& plgs)
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

void lavrentev::areaMean(std::istream&, const std::vector< Polygon >& plgs)
{
  if (!plgs.size())
  {
    return;
  }
  std::vector< float > areas(plgs.size());
  std::transform(
    plgs.begin(),
    plgs.end(),
    areas.begin(),
    std::bind(&lavrentev::Polygon::getArea, _1)
  );
  float total = std::accumulate(areas.begin(), areas.end(), 0.0f);
  std::cout << total / areas.size() << "\n";
}

bool lavrentev::isAmount(Polygon p, size_t n)
{
  return p.getSize() == n;
}

void lavrentev::areaVrtxs(const std::vector< Polygon >& plgs, size_t n)
{
  std::vector< Polygon > needPlgs;
  std::copy_if(
    plgs.begin(),
    plgs.end(),
    std::back_inserter(needPlgs),
    std::bind(isAmount, _1, n)
  );
  if (!needPlgs.size())
  {
    return;
  }
  std::vector< float > areas(needPlgs.size());
  std::transform(
    needPlgs.begin(),
    needPlgs.end(),
    areas.begin(),
    std::bind(&lavrentev::Polygon::getArea, _1)
  );
  float total = std::accumulate(areas.begin(), areas.end(), 0.0f);
  std::cout << total << "\n";
}

void lavrentev::max(std::istream& is, const std::vector< Polygon >& plgs)
{
  std::map< std::string, void (*)(std::istream&, const std::vector< Polygon >&) > cmds;
  cmds["AREA"] = maxArea;
  cmds["VERTEXES"] = maxVrtxs;
  std::string param;
  is >> param;
  if (cmds.find(param) != cmds.end())
  {
    cmds[param](is, plgs);
    return;
  }
  else
  {
    throw std::invalid_argument("Invalid command");
  }
}

void lavrentev::maxArea(std::istream&, const std::vector< Polygon >& plgs)
{
  auto mp = std::max_element(
    plgs.begin(),
    plgs.end(),
    std::bind(&lavrentev::Polygon::getArea, _1)
  );
  if (mp != plgs.end())
  {
    std::cout << (*mp).getArea() << "\n";
  }
}

const size_t lavrentev::Polygon::getSize() const
{
  return points.size();
}

void lavrentev::maxVrtxs(std::istream&, const std::vector< Polygon >& plgs)
{
  auto mp = std::max_element(
    plgs.begin(),
    plgs.end(),
    std::bind(&lavrentev::Polygon::getSize, _1)
  );
  if (mp != plgs.end())
  {
    std::cout << (*mp).getSize() << "\n";
  }
}

void lavrentev::min(std::istream& is, const std::vector< Polygon >& plgs)
{
  std::map< std::string, void (*)(std::istream&, const std::vector< Polygon >&) > cmds;
  cmds["AREA"] = minArea;
  cmds["VERTEXES"] = minVrtxs;
  std::string param;
  is >> param;
  if (cmds.find(param) != cmds.end())
  {
    cmds[param](is, plgs);
    return;
  }
  else
  {
    throw std::invalid_argument("Invalid command");
  }
}

void lavrentev::minArea(std::istream&, const std::vector< Polygon >& plgs)
{
  auto mp = std::min_element(
    plgs.begin(),
    plgs.end(),
    std::bind(&lavrentev::Polygon::getArea, _1)
  );
  if (mp != plgs.end())
  {
    std::cout << (*mp).getArea() << "\n";
  }
}

void lavrentev::minVrtxs(std::istream&, const std::vector< Polygon >& plgs)
{
  auto mp = std::min_element(
    plgs.begin(),
    plgs.end(),
    std::bind(&lavrentev::Polygon::getSize, _1)
  );
  if (mp != plgs.end())
  {
    std::cout << (*mp).getSize() << "\n";
  }
}

void lavrentev::count(std::istream& is, const std::vector< Polygon >& plgs)
{
  std::map< std::string, void (*)(std::istream&, const std::vector< Polygon >&) > cmds;
  cmds["EVEN"] = countEven;
  cmds["ODD"] = countOdd;
  std::string param;
  is >> param;
  if (cmds.find(param) != cmds.end())
  {
    cmds[param](is, plgs);
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
      countVrtxs(plgs, n);
    } else {
      throw std::invalid_argument("Invalid number");
    }
  }
  catch (...)
  {
    throw std::invalid_argument("Invalid command");
  }
}

void lavrentev::countEven(std::istream&, const std::vector< Polygon >& plgs)
{
  std::vector< Polygon > evenPlgs;
  std::copy_if(plgs.begin(), plgs.end(), std::back_inserter(evenPlgs), isEven);
  std::cout << evenPlgs.size() << "\n";
}

void lavrentev::countOdd(std::istream&, const std::vector< Polygon >& plgs)
{
  std::vector< Polygon > oddPlgs;
  std::copy_if(plgs.begin(), plgs.end(), std::back_inserter(oddPlgs), isOdd);
  std::cout << oddPlgs.size() << "\n";
}

void lavrentev::countVrtxs(const std::vector< Polygon >& plgs, size_t n)
{
  std::vector< Polygon > needPlgs;
  std::copy_if(
    plgs.begin(),
    plgs.end(),
    std::back_inserter(needPlgs),
    std::bind(isAmount, _1, n)
  );
  std::cout << needPlgs.size() << "\n";
}

const float lavrentev::Polygon::getArea() const
{
  std::vector< Triangle > triangles(getSize() - 2);
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


