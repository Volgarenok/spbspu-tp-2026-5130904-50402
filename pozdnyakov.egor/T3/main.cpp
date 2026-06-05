#include <vector>

namespace pozdnyakov
{

  struct Point
  {
    int x, y;
  };

  bool operator==(const Point &lhs, const Point &rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  bool operator!=(const Point &lhs, const Point &rhs)
  {
    return !(lhs == rhs);
  }

  struct Polygon
  {
    std::vector< Point > points;
  };

  bool operator==(const Polygon &lhs, const Polygon &rhs)
  {
    return lhs.points == rhs.points;
  }

}
