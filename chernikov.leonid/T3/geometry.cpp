#include "geometry.hpp"
#include <istream>
#include <ostream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <cmath>
#include <set>
#include <string>
#include <cstdlib>

bool chernikov::operator==(const Point &a, const Point &b)
{
  return (a.x == b.x) && (a.y == b.y);
}

bool chernikov::operator<(const Point &a, const Point &b)
{
  if (a.x != b.x)
  {
    return a.x < b.x;
  }
  return a.y < b.y;
}

std::istream &chernikov::operator>>(std::istream &in, Point &point)
{
  char openParen = '\0';
  char semicolon = '\0';
  char closeParen = '\0';

  in >> openParen >> point.x >> semicolon >> point.y >> closeParen;

  if (openParen != '(' || semicolon != ';' || closeParen != ')')
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream &chernikov::operator<<(std::ostream &out, const Point &point)
{
  out << "(" << point.x << ";" << point.y << ")";
  return out;
}

bool chernikov::operator==(const Polygon &a, const Polygon &b)
{
  if (a.points.size() != b.points.size())
  {
    return false;
  }

  if (a.points.empty())
  {
    return true;
  }

  auto it = std::search(
      a.points.begin(), a.points.end(),
      b.points.begin(), b.points.end());

  if (it != a.points.end())
  {
    return true;
  }

  std::vector<Point> doubled(a.points.begin(), a.points.end());
  doubled.insert(doubled.end(), a.points.begin(), a.points.end());

  it = std::search(
      doubled.begin(), doubled.end(),
      b.points.begin(), b.points.end());

  return it != doubled.end();
}

bool chernikov::operator<(const Polygon &a, const Polygon &b)
{
  if (a.points.size() != b.points.size())
  {
    return a.points.size() < b.points.size();
  }
  return std::lexicographical_compare(
      a.points.begin(), a.points.end(),
      b.points.begin(), b.points.end());
}

std::istream &chernikov::operator>>(std::istream &in, Polygon &polygon)
{
  int vertexCount = 0;
  in >> vertexCount;

  if (!in || vertexCount < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector<Point> temp;
  temp.reserve(vertexCount);

  std::generate_n(std::back_inserter(temp), vertexCount,
                  [&in]()
                  {
                    Point p;
                    in >> p;
                    return p;
                  });

  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  polygon.points = std::move(temp);
  return in;
}

std::ostream &chernikov::operator<<(std::ostream &out, const Polygon &polygon)
{
  out << polygon.points.size();
  if (!polygon.points.empty())
  {
    out << " ";
    std::copy(
        polygon.points.begin(),
        polygon.points.end(),
        std::ostream_iterator<Point>(out, " "));
  }
  return out;
}

double chernikov::computeArea(const Polygon &polygon)
{
  if (polygon.points.size() < 3)
  {
    return 0.0;
  }

  std::vector<int> products(polygon.points.size());

  std::transform(
      polygon.points.begin(),
      polygon.points.end(),
      products.begin(),
      [&polygon, i = 0u](const Point &current) mutable
      {
        const Point &next = polygon.points[(i + 1) % polygon.points.size()];
        int result = current.x * next.y - current.y * next.x;
        ++i;
        return result;
      });

  int sum = std::accumulate(products.begin(), products.end(), 0);
  return std::abs(sum) / 2.0;
}

bool chernikov::isRectangle(const Polygon &polygon)
{
  if (polygon.points.size() != 4)
  {
    return false;
  }

  auto dotProduct = [](const Point &a, const Point &b)
  {
    return a.x * b.x + a.y * b.y;
  };

  auto subtract = [](const Point &a, const Point &b)
  {
    return Point{a.x - b.x, a.y - b.y};
  };

  Point ab = subtract(polygon.points[1], polygon.points[0]);
  Point bc = subtract(polygon.points[2], polygon.points[1]);
  Point cd = subtract(polygon.points[3], polygon.points[2]);
  Point da = subtract(polygon.points[0], polygon.points[3]);

  return (dotProduct(ab, bc) == 0) && (dotProduct(bc, cd) == 0) && (dotProduct(cd, da) == 0) && (dotProduct(da, ab) == 0);
}

bool chernikov::hasRightAngle(const Polygon &polygon)
{
  if (polygon.points.size() < 3)
  {
    return false;
  }

  auto dotProduct = [](const Point &a, const Point &b)
  {
    return a.x * b.x + a.y * b.y;
  };

  auto subtract = [](const Point &a, const Point &b)
  {
    return Point{a.x - b.x, a.y - b.y};
  };

  std::vector<bool> results(polygon.points.size());

  std::transform(
      polygon.points.begin(),
      polygon.points.end(),
      results.begin(),
      [&polygon, &dotProduct, &subtract, i = 0u](const Point &) mutable
      {
        size_t first = i;
        size_t second = (i + 1) % polygon.points.size();
        size_t third = (i + 2) % polygon.points.size();

        Point v1 = subtract(polygon.points[second], polygon.points[first]);
        Point v2 = subtract(polygon.points[third], polygon.points[second]);

        ++i;
        return dotProduct(v1, v2) == 0;
      });

  return std::any_of(results.begin(), results.end(), [](bool val)
                     { return val; });
}

bool chernikov::isPermutationOf(const Polygon &a, const Polygon &b)
{
  if (a.points.size() != b.points.size())
  {
    return false;
  }

  std::multiset<Point> setA(a.points.begin(), a.points.end());
  std::multiset<Point> setB(b.points.begin(), b.points.end());

  return setA == setB;
}
