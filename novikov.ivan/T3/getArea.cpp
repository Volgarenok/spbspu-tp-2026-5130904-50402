#include "getArea.hpp"
#include <algorithm>
#include <cstddef>
#include <numeric>

namespace
{
  struct CrossProductFunctor
  {
  public:
    explicit CrossProductFunctor(const std::vector< novikov::Point >& pointsVec):
        points(pointsVec)
    {}

    double operator()(size_t i) const
    {
      size_t next = (i + 1) % points.size();
      return static_cast< double >(points[i].x * points[next].y - points[next].x * points[i].y);
    }

  private:
    const std::vector< novikov::Point >& points;
  };
}

double novikov::getPolygonArea(const Polygon& p)
{
  if (p.points.empty())
  {
    return 0;
  }
  std::vector< size_t > indices(p.points.size());
  std::iota(indices.begin(), indices.end(), 0);
  std::vector< double > partialAreas(p.points.size());
  std::transform(indices.begin(), indices.end(), partialAreas.begin(), CrossProductFunctor{p.points});
  return std::abs(std::accumulate(partialAreas.begin(), partialAreas.end(), 0.0)) / 2;
}
