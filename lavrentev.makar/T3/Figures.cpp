#include <numeric>
#include <cmath>
#include <algorithm>
#include "Figures.hpp"

using namespace std::placeholders;

bool lavrentev::isEven(Polygon p)
{
  return p.getSize() % 2 == 0;
}

bool lavrentev::isOdd(Polygon p)
{
  return !isEven(p);
}

bool lavrentev::isAmount(Polygon p, size_t n)
{
  return p.getSize() == n;
}

const size_t lavrentev::Polygon::getSize() const
{
  return points.size();
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


