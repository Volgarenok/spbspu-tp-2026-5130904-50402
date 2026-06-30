#include "UserCommands.hpp"
#include <numeric>
#include <algorithm>
#include <iostream>
#include <functional>
#include <iterator>

using namespace std::placeholders;

void lavrentev::maxseq(std::istream &is, const std::vector< Polygon > &plgs)
{
  Polygon p;
  is >> p;

  if (p.isEmpty() || !is)
  {
    throw std::invalid_argument("Invalid data");
  }

  std::vector< size_t > lengths(plgs.size());
  std::iota(lengths.begin(), lengths.end(), 1);

  std::vector< bool > checks(plgs.size());

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

int lavrentev::orient(const Point& a, const Point& b, const Point& c)
{
  long long val = 1LL * (b.x - a.x) * (c.y - a.y) - 1LL * (b.y - a.y) * (c.x - a.x);
  if (val == 0)
  {
    return 0;
  }
  return (val > 0) ? 1 : 2;
}

bool lavrentev::segInter(
  const Point& p1,
  const Point& q1,
  const Point& p2,
  const Point& q2)
{
  int o1 = orient(p1, q1, p2);
  int o2 = orient(p1, q1, q2);
  int o3 = orient(p2, q2, p1);
  int o4 = orient(p2, q2, q1);

  if (o1 != o2 && o3 != o4)
  {
    return true;
  }

  return false;
}

bool lavrentev::checkPair(
  size_t j,
  const std::vector< Point >* ptsB,
  const Point* a1,
  const Point* a2)
{
  const Point& b1 = (*ptsB)[j];
  const Point& b2 = (*ptsB)[(j + 1) % ptsB->size()];

  return segInter(*a1, *a2, b1, b2);
}

bool lavrentev::edgeInterAll(
  size_t idxA,
  const Polygon* A,
  const Polygon* B)
{
  const auto& ptsA = A->points;
  const auto& ptsB = B->points;

  const Point& a1 = ptsA[idxA];
  const Point& a2 = ptsA[(idxA + 1) % ptsA.size()];

  std::vector< size_t > idxB(ptsB.size());
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

size_t lavrentev::rayAccum(
  size_t acc,
  const Point&,
  const std::vector< Point >* pts,
  const Point* pt,
  size_t* index)
{
  const auto& a = (*pts)[*index];
  const auto& b = (*pts)[(*index + 1) % pts->size()];

  ++(*index);

  if ((a.y > pt->y) != (b.y > pt->y))
  {
    double x = a.x + static_cast< double >(pt->y - a.y) * (b.x - a.x) / (b.y - a.y);

    if (x > pt->x)
    {
      return acc + 1;
    }
  }

  return acc;
}

bool lavrentev::pInside(const Polygon& poly, const Point& pt)
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

bool lavrentev::polyInter(const Polygon& A, const Polygon& B)
{
  if (A.points.empty() || B.points.empty())
  {
    return false;
  }

  std::vector< size_t > idxA(A.points.size());
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

void lavrentev::intersections(std::istream& is, const std::vector< Polygon >& plgs)
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
