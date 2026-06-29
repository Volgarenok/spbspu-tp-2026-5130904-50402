#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
#include "IOguard.hpp"
#include "commands.hpp"

namespace
{
  bool isRectangle(const shirokov::Polygon&);
  int getSquaredDistance(const shirokov::Point&, const shirokov::Point&);

  int getSquaredDistance(const shirokov::Point& a, const shirokov::Point& b)
  {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
  }

  bool isRectangle(const shirokov::Polygon& p)
  {
    if (p.points.size() != 4)
    {
      return false;
    }

    int d1 = getSquaredDistance(p.points[0], p.points[1]);
    int d2 = getSquaredDistance(p.points[1], p.points[2]);
    int d3 = getSquaredDistance(p.points[2], p.points[3]);
    int d4 = getSquaredDistance(p.points[3], p.points[0]);

    int diag1 = getSquaredDistance(p.points[0], p.points[2]);
    int diag2 = getSquaredDistance(p.points[1], p.points[3]);

    return (d1 > 0 && d2 > 0 && d3 > 0 && d4 > 0) && (d1 == d3 && d2 == d4) && (diag1 == diag2) && (diag1 == d1 + d2);
  }
}

void shirokov::rects(std::istream&, std::ostream& out, shirokov::plg_t& polygons)
{
  shirokov::IOguard g(out);
  out << std::fixed << std::setprecision(1);

  out << std::count_if(polygons.begin(), polygons.end(), isRectangle) << "\n";
}
