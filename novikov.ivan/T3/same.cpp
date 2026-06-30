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

  using pvec = std::vector< novikov::Point >;
  bool checkShift(const pvec& a, const pvec& b, size_t shift, int dx, int dy)
  {
    size_t n = a.size();
    for (size_t i = 0; i < n; ++i)
    {
      const auto& pA = a[(i + shift) % n];
      const auto& pB = b[i];
      if ((pA.x + dx != pB.x) || (pA.y + dy != pB.y))
      {
        return false;
      }
    }
    return true;
  }

  struct SamePredicate
  {
  public:
    explicit SamePredicate(const novikov::Polygon& target):
      targetPoly(target)
    {}

    bool operator()(const novikov::Polygon& p) const
    {
      if (p.points.size() != targetPoly.points.size())
      {
        return false;
      }
      if (p.points.empty())
      {
        return true;
      }

      size_t n = p.points.size();
      for (size_t shift = 0; shift < n; ++shift)
      {
        int dx = targetPoly.points[0].x - p.points[shift].x;
        int dy = targetPoly.points[0].y - p.points[shift].y;

        if (checkShift(p.points, targetPoly.points, shift, dx, dy))
        {
          return true;
        }
      }
      return false;
    }

  private:
    const novikov::Polygon& targetPoly;
  };
}

void novikov::same(std::istream& in, std::ostream& out, novikov::plg_t& polygons)
{
  novikov::Polygon target;
  if (!(in >> target))
  {
    throw std::logic_error("Invalid polygon format");
  }

  char ch = 0;
  while (in.peek() != '\n' && !in.eof())
  {
    in.get(ch);
    if (isNotSpace(ch))
    {
      in.setstate(std::ios::failbit);
      throw std::logic_error("Trailing garbage after polygon");
    }
  }

  novikov::IOGuard g(out);
  out << std::fixed << std::setprecision(1);

  out << std::count_if(polygons.begin(), polygons.end(), SamePredicate{target});
}
