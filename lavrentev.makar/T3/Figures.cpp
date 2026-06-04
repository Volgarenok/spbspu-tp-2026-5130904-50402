#include <numeric>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <functional>
#include <iterator>
#include <limits>
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

size_t lavrentev::Polygon::getSize() const
{
  return points.size();
}

float lavrentev::Polygon::getArea() const
{
  if (getSize() < 3)
  {
    return 0.0f;
  }
  std::vector<Triangle> triangles(getSize() - 2);
  size_t index = 1;
  std::generate(
    triangles.begin(),
    triangles.end(),
    std::bind(&lavrentev::Polygon::helpArea, std::ref(points), std::ref(index))
  );

  std::vector<float> areas(triangles.size());
  std::transform(
    triangles.begin(),
    triangles.end(),
    areas.begin(),
    std::bind(&lavrentev::Triangle::getArea, _1)
  );
  float total = std::accumulate(areas.begin(), areas.end(), 0.0f);

  return total;
}

lavrentev::Triangle lavrentev::Polygon::helpArea(const std::vector<Point> &points, size_t &index)
{
  Triangle ans = Triangle{points[0], points[index], points[index + 1]};
  ++index;
  return ans;
}

float lavrentev::Triangle::getArea() const
{
  int x1 = points[0].x;
  int x2 = points[1].x;
  int x3 = points[2].x;
  int y1 = points[0].y;
  int y2 = points[1].y;
  int y3 = points[2].y;

  return 0.5 * std::abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
}

void lavrentev::maxseq(std::istream &is, const std::vector<Polygon> &plgs)
{
  Polygon p;
  is >> p;

  if (p.isEmpty())
  {
    throw std::invalid_argument("Invalid data");
  }

  std::vector<size_t> lengths(plgs.size());
  std::iota(lengths.begin(), lengths.end(), 1);

  auto it = std::partition_point(
    lengths.begin(),
    lengths.end(),
    std::bind(helpMS, _1, std::ref(plgs), std::ref(p))
  );

  size_t ms = std::distance(lengths.begin(), it);
  std::cout << ms << "\n";
}

bool lavrentev::helpMS(int n, const std::vector<Polygon> &plgs, const Polygon &p)
{
  return std::search_n(
    plgs.begin(),
    plgs.end(),
    n,
    p,
    std::equal_to<Polygon>{}) != plgs.end();
}

bool lavrentev::Polygon::operator==(const Polygon &p) const
{
  return points == p.points;
}

std::istream& lavrentev::operator>>(std::istream& is, Polygon& plg)
{
  std::istream::sentry s(is);
  if (!s)
  {
    return is;
  }

  size_t n = 0;
  if (!(is >> n) || n < 3)
  {
    is.clear();
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    plg.points.clear();
    return is;
  }

  std::vector<Point> temp;
  temp.reserve(n);

  std::copy_n(
    std::istream_iterator<Point>(is),
    n,
    std::back_inserter(temp)
  );

  if (temp.size() != n)
  {
    is.clear();
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    plg.points.clear();
    return is;
  }

  std::string tail;
  std::getline(is, tail);

  if (tail.find_first_not_of(" \t\r") != std::string::npos)
  {
    is.clear();
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    plg.points.clear();
    return is;
  }

  plg.points = std::move(temp);
  return is;
}

std::istream &lavrentev::operator>>(std::istream &is, Point &p)
{
  std::istream::sentry s(is);
  if (!s)
  {
    return is;
  }
  using d_t = Delimiter_t;
  char last = 0;
  d_t dlmInBracket{'(', last};
  d_t dlmColon{';', last};
  d_t dlmOutBracket{')', last};
  int x, y;
  is >> dlmInBracket >> x >> dlmColon >> y >> dlmOutBracket;
  if (!is)
  {
    return is;
  }
  p = Point{x, y};
  return is;
}

char lavrentev::check(std::istream &is, char expected)
{
  char c = 0;
  is >> c;
  if (c != expected)
  {
    is.setstate(std::ios_base::failbit);
  }
  return c;
}

std::istream &lavrentev::operator>>(std::istream &is, Delimiter_t &del)
{
  del.last = lavrentev::check(is, del.expected);
  return is;
}

bool lavrentev::Point::operator==(const Point &p) const
{
  return (x == p.x) && (y == p.y);
}

bool lavrentev::Polygon::isEmpty() const
{
  return getSize() == 0;
}

bool lavrentev::Point::operator<(const Point &other) const
{
  bool c1 = x < other.x;
  bool c2 = x == other.x && y < other.y;
  return c1 || c2;
}

void lavrentev::intersections(std::istream &, const std::vector<Polygon> &)
{
}
