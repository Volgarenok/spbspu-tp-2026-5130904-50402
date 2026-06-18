#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "commands.hpp"
#include "parser.hpp"

namespace
{
  bool isPointEqual(const shirokov::Point&, const shirokov::Point&);
  bool isPolygonEqual(const shirokov::Polygon&, const shirokov::Polygon&);

  struct SequenceTracker
  {
  public:
    SequenceTracker(const shirokov::Polygon& target, size_t& current, size_t& maxSeq):
      targetPoly(target),
      currentCount(current),
      maxCount(maxSeq)
    {}

    size_t operator()(const shirokov::Polygon& p)
    {
      if (isPolygonEqual(p, targetPoly))
      {
        ++currentCount;
        if (currentCount > maxCount)
        {
          maxCount = currentCount;
        }
      }
      else
      {
        currentCount = 0;
      }
      return maxCount;
    }

  private:
    const shirokov::Polygon& targetPoly;
    size_t& currentCount;
    size_t& maxCount;
  };

  bool isPointEqual(const shirokov::Point& a, const shirokov::Point& b)
  {
    return a.x == b.x && a.y == b.y;
  }

  bool isPolygonEqual(const shirokov::Polygon& a, const shirokov::Polygon& b)
  {
    if (a.points.size() != b.points.size())
    {
      return false;
    }
    return std::equal(a.points.begin(), a.points.end(), b.points.begin(), isPointEqual);
  }
}

void shirokov::maxseq(std::istream& in, std::ostream& out, shirokov::plg_t& polygons)
{
  shirokov::Polygon target;
  if (!(in >> target))
  {
    throw std::logic_error("Invalid polygon format");
  }

  shirokov::IOguard g(out);
  out << std::fixed << std::setprecision(1);

  size_t currentCount = 0;
  size_t maxCount = 0;

  std::vector< size_t > dummy(polygons.size());
  std::transform(polygons.begin(), polygons.end(), dummy.begin(), SequenceTracker{target, currentCount, maxCount});

  out << maxCount << "\n";
}
