#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "commands.hpp"
#include "Parser.hpp"
#include "IOGuard.hpp"

namespace
{
  bool isNotSpace(char c)
  {
    return std::isspace(static_cast< unsigned char >(c)) == 0;
  }

  struct Edge
  {
    novikov::Point p1, p2;
  };

  int orientation(novikov::Point p, novikov::Point q, novikov::Point r)
  {
    long long val = 1LL * (q.y - p.y) * (r.x - q.x) - 1LL * (q.x - p.x) * (r.y - q.y);
    if (val == 0) {
      return 0;
    }
    return (val > 0) ? 1 : 2;
  }

  bool onSegment(novikov::Point p, novikov::Point q, novikov::Point r)
  {
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x)
        && q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
  }

  bool intersectsEdges(Edge e1, Edge e2)
  {
    int o1 = orientation(e1.p1, e1.p2, e2.p1);
    int o2 = orientation(e1.p1, e1.p2, e2.p2);
    int o3 = orientation(e2.p1, e2.p2, e1.p1);
    int o4 = orientation(e2.p1, e2.p2, e1.p2);

    if (o1 != o2 && o3 != o4) return true;

    if (o1 == 0 && onSegment(e1.p1, e2.p1, e1.p2)) return true;
    if (o2 == 0 && onSegment(e1.p1, e2.p2, e1.p2)) return true;
    if (o3 == 0 && onSegment(e2.p1, e1.p1, e2.p2)) return true;
    if (o4 == 0 && onSegment(e2.p1, e1.p2, e2.p2)) return true;

    return false;
  }

  bool isPointInPolygon(novikov::Point p, const novikov::Polygon& poly)
  {
    size_t n = poly.points.size();
    if (n < 3) return false;

    bool inside = false;
    for (size_t i = 0, j = n - 1; i < n; j = i++)
    {
      if (((poly.points[i].y > p.y) != (poly.points[j].y > p.y)) &&
          (p.x < 1.0 * (poly.points[j].x - poly.points[i].x) * (p.y - poly.points[i].y)
              / (poly.points[j].y - poly.points[i].y) + poly.points[i].x))
      {
        inside = !inside;
      }
    }
    return inside;
  }

  bool doPolygonsIntersect(const novikov::Polygon& a, const novikov::Polygon& b)
  {
    for (size_t i = 0; i < a.points.size(); ++i)
    {
      Edge e1{ a.points[i], a.points[(i + 1) % a.points.size()] };
      for (size_t j = 0; j < b.points.size(); ++j)
      {
        Edge e2{ b.points[j], b.points[(j + 1) % b.points.size()] };
        if (intersectsEdges(e1, e2))
        {
          return true;
        }
      }
    }

    if (!a.points.empty() && isPointInPolygon(a.points[0], b)) return true;
    if (!b.points.empty() && isPointInPolygon(b.points[0], a)) return true;

    return false;
  }

  struct IntersectionsPredicate
  {
  public:
    explicit IntersectionsPredicate(const novikov::Polygon& target):
        targetPoly(target)
    {}

    bool operator()(const novikov::Polygon& p) const
    {
      return doPolygonsIntersect(targetPoly, p);
    }

  private:
    const novikov::Polygon& targetPoly;
  };
}

void novikov::intersections(std::istream& in, std::ostream& out, novikov::plg_t& polygons)
{
  novikov::Polygon target;
  if (!(in >> target))
  {
    throw std::logic_error("Invalid polygon format");
  }

  std::string restOfLine;
  if (std::getline(in, restOfLine))
  {
    if (std::any_of(restOfLine.begin(), restOfLine.end(), isNotSpace))
    {
      in.setstate(std::ios::failbit);
      throw std::logic_error("Trailing garbage after polygon");
    }
  }

  novikov::IOGuard g(out);
  out << std::fixed << std::setprecision(1);

  out << std::count_if(polygons.begin(), polygons.end(), IntersectionsPredicate{target});
}
