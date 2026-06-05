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

  if (p.isEmpty() || !is)
  {
    throw std::invalid_argument("Invalid data");
  }

  std::vector<size_t> lengths(plgs.size());
  std::iota(lengths.begin(), lengths.end(), 1);

  std::vector<bool> checks(plgs.size());

  std::transform(
    lengths.begin(),
    lengths.end(),
    checks.begin(),
    std::bind(helpMS, _1, std::ref(plgs), std::ref(p))
  );

  auto it = std::find(checks.rbegin(), checks.rend(), true);

  size_t result = (it == checks.rend()) ? 0 : checks.size() - std::distance(checks.rbegin(), it);

  std::cout << result << "\n";
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

  std::generate_n(
    std::back_inserter(temp),
    n,
    std::bind(readPoint, &is)
  );

  if (!is)
  {
    is.clear();
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    plg.points.clear();
    return is;
  }

  std::string tail;
  std::getline(is, tail);

  if (tail.find_first_not_of(" \t\r\n") != std::string::npos)
  {
    is.setstate(std::ios_base::failbit);
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

lavrentev::Point lavrentev::readPoint(std::istream* is)
  {
    lavrentev::Point p;
    (*is) >> p;
    return p;
  }

int orient(const lavrentev::Point& a, const lavrentev::Point& b, const lavrentev::Point& c)
{
  long long val = 1LL * (b.x - a.x) * (c.y - a.y) - 1LL * (b.y - a.y) * (c.x - a.x);

  if (val == 0)
  {
    return 0;
  }
  return (val > 0) ? 1 : 2;
}

bool segInter(
  const lavrentev::Point& p1,
  const lavrentev::Point& q1,
  const lavrentev::Point& p2,
  const lavrentev::Point& q2)
{
  int o1 = orient(p1, q1, p2);
  int o2 = orient(p1, q1, q2);
  int o3 = orient(p2, q2, p1);
  int o4 = orient(p2, q2, q1);

  return (o1 != o2 && o3 != o4);
}

bool checkPair(
  size_t j,
  const std::vector<lavrentev::Point>* ptsB,
  const lavrentev::Point* a1,
  const lavrentev::Point* a2)
{
  const lavrentev::Point& b1 = (*ptsB)[j];
  const lavrentev::Point& b2 = (*ptsB)[(j + 1) % ptsB->size()];

  return segInter(*a1, *a2, b1, b2);
}

bool edgeInterAll(
  size_t idxA,
  const lavrentev::Polygon* A,
  const lavrentev::Polygon* B)
{
  const auto& ptsA = A->points;
  const auto& ptsB = B->points;

  const lavrentev::Point& a1 = ptsA[idxA];
  const lavrentev::Point& a2 = ptsA[(idxA + 1) % ptsA.size()];

  std::vector<size_t> idxB(ptsB.size());
  std::iota(idxB.begin(), idxB.end(), 0);

  return std::any_of(
    idxB.begin(),
    idxB.end(),
    std::bind(
      checkPair,
      std::placeholders::_1,
      &ptsB,
      &a1,
      &a2
    )
  );
}

size_t rayAccum(
  size_t acc,
  const lavrentev::Point&,
  const std::vector<lavrentev::Point>* pts,
  const lavrentev::Point* pt,
  size_t* index)
{
  const auto& a = (*pts)[*index];
  const auto& b = (*pts)[(*index + 1) % pts->size()];

  ++(*index);

  if ((a.y > pt->y) != (b.y > pt->y))
  {
    double x = a.x + static_cast<double>(pt->y - a.y) * (b.x - a.x) / (b.y - a.y);

    if (x > pt->x)
    {
      return acc + 1;
    }
  }

  return acc;
}

bool pInside(const lavrentev::Polygon& poly, const lavrentev::Point& pt)
{
  if (poly.points.empty())
  {
    return false;
  }

  const auto& pts = poly.points;

  size_t index = 0;

  size_t count = std::accumulate(
    pts.begin(),
    pts.end(),
    size_t(0),
    std::bind(
      rayAccum,
      std::placeholders::_1,
      std::placeholders::_2,
      &pts,
      &pt,
      &index
    )
  );

  return count % 2 == 1;
}

bool polyInter(const lavrentev::Polygon& A, const lavrentev::Polygon& B)
{
  if (A.points.empty() || B.points.empty())
  {
    return false;
  }

  std::vector<size_t> idxA(A.points.size());
  std::iota(idxA.begin(), idxA.end(), 0);

  bool edges = std::any_of(
    idxA.begin(),
    idxA.end(),
    std::bind(
      edgeInterAll,
      std::placeholders::_1,
      &A,
      &B
    )
  );

  if (edges)
  {
    return true;
  }
  if (pInside(A, B.points.front()))
  {
    return true;
  }
  if (pInside(B, A.points.front()))
  {
    return true;
  }

  return false;
}

void lavrentev::intersections(std::istream& is, const std::vector<Polygon>& plgs)
{
  Polygon p;
  is >> p;

  if (p.isEmpty() || !is)
  {
    throw std::invalid_argument("Invalid data");
  }

  size_t count = std::count_if(
    plgs.begin(),
    plgs.end(),
    std::bind(
      polyInter,
      std::placeholders::_1,
      std::cref(p)
    )
  );

  std::cout << count << "\n";
}
