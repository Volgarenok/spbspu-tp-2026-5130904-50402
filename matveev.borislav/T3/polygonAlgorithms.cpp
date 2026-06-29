#include "polygonAlgorithms.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace
{
  bool isLessX(const matveev::Point& lhs, const matveev::Point& rhs)
  {
    return lhs.x < rhs.x;
  }

  bool isLessY(const matveev::Point& lhs, const matveev::Point& rhs)
  {
    return lhs.y < rhs.y;
  }

  bool isLessFrameLeftX(const matveev::Frame& lhs, const matveev::Frame& rhs)
  {
    return lhs.left_bottom.x < rhs.left_bottom.x;
  }

  bool isLessFrameBottomY(const matveev::Frame& lhs, const matveev::Frame& rhs)
  {
    return lhs.left_bottom.y < rhs.left_bottom.y;
  }

  bool isLessFrameRightX(const matveev::Frame& lhs, const matveev::Frame& rhs)
  {
    return lhs.right_top.x < rhs.right_top.x;
  }

  bool isLessFrameTopY(const matveev::Frame& lhs, const matveev::Frame& rhs)
  {
    return lhs.right_top.y < rhs.right_top.y;
  }

  long long getOrientation(
    const matveev::Point& first,
    const matveev::Point& second,
    const matveev::Point& third
  )
  {
    long long first_x = second.x - first.x;
    long long first_y = second.y - first.y;
    long long second_x = third.x - first.x;
    long long second_y = third.y - first.y;

    return first_x * second_y - first_y * second_x;
  }

  bool isBetween(int left, int value, int right)
  {
    return value >= std::min(left, right) && value <= std::max(left, right);
  }

  bool isPointOnSegment(const matveev::Edge& edge, const matveev::Point& point)
  {
    return getOrientation(edge.first, edge.second, point) == 0
      && isBetween(edge.first.x, point.x, edge.second.x)
      && isBetween(edge.first.y, point.y, edge.second.y);
  }

  bool isEdgeIntersectWith(const matveev::Edge& first, const matveev::Edge& second)
  {
    return matveev::isSegmentsIntersect(first, second);
  }
}

matveev::detail::TriangleAreaGenerator::TriangleAreaGenerator(
  const Point& base,
  std::vector< Point >::const_iterator current
):
  base_(base),
  current_(current)
{}

double matveev::detail::TriangleAreaGenerator::operator()()
{
  std::vector< Point >::const_iterator next = current_;
  ++next;

  double area = getTriangleArea(base_, *current_, *next);

  ++current_;

  return area;
}

matveev::detail::EdgeGenerator::EdgeGenerator(const std::vector< Point >& points):
  points_(points),
  index_(0)
{}

matveev::Edge matveev::detail::EdgeGenerator::operator()()
{
  std::size_t next_index = index_ + 1;

  if (next_index == points_.size())
  {
    next_index = 0;
  }

  Edge edge = { points_[index_], points_[next_index] };
  ++index_;

  return edge;
}

matveev::detail::IntersectWithEdges::IntersectWithEdges(const std::vector< Edge >& edges):
  edges_(edges)
{}

bool matveev::detail::IntersectWithEdges::operator()(const Edge& edge) const
{
  using namespace std::placeholders;
  return std::any_of(edges_.begin(), edges_.end(), std::bind(isEdgeIntersectWith, edge, _1));
}

matveev::detail::PointOnEdge::PointOnEdge(const Point& point):
  point_(point)
{}

bool matveev::detail::PointOnEdge::operator()(const Edge& edge) const
{
  return isPointOnSegment(edge, point_);
}

matveev::detail::RayCrossCounter::RayCrossCounter(const Point& point):
  point_(point)
{}

bool matveev::detail::RayCrossCounter::operator()(const Edge& edge) const
{
  const Point& first = edge.first;
  const Point& second = edge.second;

  bool crosses_y = (first.y > point_.y) != (second.y > point_.y);

  if (!crosses_y)
  {
    return false;
  }

  long long x = static_cast< long long >(second.x - first.x) * (point_.y - first.y);
  long long y = static_cast< long long >(second.y - first.y) * (point_.x - first.x);

  if (second.y > first.y)
  {
    return x > y;
  }

  return x < y;
}

matveev::detail::PointInsidePolygon::PointInsidePolygon(const Polygon& polygon):
  polygon_(polygon)
{}

bool matveev::detail::PointInsidePolygon::operator()(const Point& point) const
{
  return isPointInPolygon(point, polygon_);
}

double matveev::getTriangleArea(const Point& first, const Point& second, const Point& third)
{
  long long first_x = second.x - first.x;
  long long first_y = second.y - first.y;
  long long second_x = third.x - first.x;
  long long second_y = third.y - first.y;

  long long double_area = first_x * second_y - first_y * second_x;

  return std::abs(static_cast< double >(double_area)) / 2.0;
}

double matveev::getPolygonArea(const Polygon& polygon)
{
  if (polygon.points.size() < 3)
  {
    return 0.0;
  }

  std::vector< double > areas;
  areas.reserve(polygon.points.size() - 2);

  std::generate_n(
    std::back_inserter(areas),
    polygon.points.size() - 2,
    detail::TriangleAreaGenerator(polygon.points.front(), ++polygon.points.begin())
  );

  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

matveev::Frame matveev::getFrame(const Polygon& polygon)
{
  if (polygon.points.empty())
  {
    throw std::logic_error("empty polygon");
  }

  std::vector< Point >::const_iterator min_x = std::min_element(polygon.points.begin(), polygon.points.end(), isLessX);
  std::vector< Point >::const_iterator max_x = std::max_element(polygon.points.begin(), polygon.points.end(), isLessX);
  std::vector< Point >::const_iterator min_y = std::min_element(polygon.points.begin(), polygon.points.end(), isLessY);
  std::vector< Point >::const_iterator max_y = std::max_element(polygon.points.begin(), polygon.points.end(), isLessY);

  Frame frame = { Point{ min_x->x, min_y->y }, Point{ max_x->x, max_y->y } };
  return frame;
}

matveev::Frame matveev::getFrame(const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("empty polygons");
  }

  std::vector< Frame > frames;
  frames.reserve(polygons.size());

  std::transform(
    polygons.begin(),
    polygons.end(),
    std::back_inserter(frames),
    static_cast< Frame(*)(const Polygon&) >(getFrame)
  );

  std::vector< Frame >::const_iterator min_x = std::min_element(frames.begin(), frames.end(), isLessFrameLeftX);
  std::vector< Frame >::const_iterator min_y = std::min_element(frames.begin(), frames.end(), isLessFrameBottomY);
  std::vector< Frame >::const_iterator max_x = std::max_element(frames.begin(), frames.end(), isLessFrameRightX);
  std::vector< Frame >::const_iterator max_y = std::max_element(frames.begin(), frames.end(), isLessFrameTopY);

  Frame frame = {
    Point{ min_x->left_bottom.x, min_y->left_bottom.y },
    Point{ max_x->right_top.x, max_y->right_top.y }
  };

  return frame;
}

bool matveev::isPointInFrame(const Frame& frame, const Point& point)
{
  return point.x >= frame.left_bottom.x
    && point.x <= frame.right_top.x
    && point.y >= frame.left_bottom.y
    && point.y <= frame.right_top.y;
}

bool matveev::isPolygonInFrame(const Frame& frame, const Polygon& polygon)
{
  using namespace std::placeholders;
  return std::all_of(polygon.points.begin(), polygon.points.end(), std::bind(isPointInFrame, frame, _1));
}

std::vector< matveev::Edge > matveev::getEdges(const Polygon& polygon)
{
  std::vector< Edge > edges;
  edges.reserve(polygon.points.size());

  std::generate_n(std::back_inserter(edges), polygon.points.size(), detail::EdgeGenerator(polygon.points));

  return edges;
}

bool matveev::isSegmentsIntersect(const Edge& first, const Edge& second)
{
  long long first_orientation = getOrientation(first.first, first.second, second.first);
  long long second_orientation = getOrientation(first.first, first.second, second.second);
  long long third_orientation = getOrientation(second.first, second.second, first.first);
  long long fourth_orientation = getOrientation(second.first, second.second, first.second);

  if (first_orientation == 0 && isPointOnSegment(first, second.first))
  {
    return true;
  }

  if (second_orientation == 0 && isPointOnSegment(first, second.second))
  {
    return true;
  }

  if (third_orientation == 0 && isPointOnSegment(second, first.first))
  {
    return true;
  }

  if (fourth_orientation == 0 && isPointOnSegment(second, first.second))
  {
    return true;
  }

  return (first_orientation > 0) != (second_orientation > 0)
    && (third_orientation > 0) != (fourth_orientation > 0);
}

bool matveev::isPolygonIntersect(const Polygon& first, const Polygon& second)
{
  std::vector< Edge > first_edges = getEdges(first);
  std::vector< Edge > second_edges = getEdges(second);

  bool has_intersected_edges = std::any_of(
    first_edges.begin(),
    first_edges.end(),
    detail::IntersectWithEdges(second_edges)
  );

  bool first_inside_second = std::any_of(
    first.points.begin(),
    first.points.end(),
    detail::PointInsidePolygon(second)
  );

  bool second_inside_first = std::any_of(
    second.points.begin(),
    second.points.end(),
    detail::PointInsidePolygon(first)
  );

  return has_intersected_edges || first_inside_second || second_inside_first;
}

bool matveev::isPointOnPolygon(const Point& point, const Polygon& polygon)
{
  std::vector< Edge > edges = getEdges(polygon);
  return std::any_of(edges.begin(), edges.end(), detail::PointOnEdge(point));
}

bool matveev::isPointInPolygon(const Point& point, const Polygon& polygon)
{
  if (isPointOnPolygon(point, polygon))
  {
    return true;
  }

  std::vector< Edge > edges = getEdges(polygon);
  std::size_t count = std::count_if(edges.begin(), edges.end(), detail::RayCrossCounter(point));

  return count % 2 == 1;
}
