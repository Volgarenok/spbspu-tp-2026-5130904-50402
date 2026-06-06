#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <istream>
#include <vector>
namespace petrov
{
  struct Point
  {
    int x, y;
  };
  bool operator==(const Point& lhs, const Point& rhs);

  struct Polygon
  {
    std::vector< Point > points;
  };

  struct DelimIO
  {
    char expected;
  };

  std::istream& operator>>(std::istream& in, DelimIO&& delim);

  class IOguard
  {
  public:
    explicit IOguard(std::istream& stream);
    ~IOguard();

  private:
    std::istream& stream_;
    std::ios::fmtflags flags_;
  };

  std::istream& operator>>(std::istream& in, Point& point);
  std::istream& operator>>(std::istream& in, Polygon& polygon);
}
