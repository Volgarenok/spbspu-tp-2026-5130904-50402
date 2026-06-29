#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "IOguard.hpp"
#include "commands.hpp"

namespace
{
  bool isCharDigit(char);
  bool isEvenVertexCount(const shirokov::Polygon&);
  bool isOddVertexCount(const shirokov::Polygon&);
  void countEven(std::ostream&, const shirokov::plg_t&);
  void countOdd(std::ostream&, const shirokov::plg_t&);
  void countNum(std::ostream&, const shirokov::plg_t&, size_t);

  struct NumVertexPredicate
  {
  public:
    explicit NumVertexPredicate(size_t targetValue):
      target(targetValue)
    {}

    bool operator()(const shirokov::Polygon& p) const
    {
      return p.points.size() == target;
    }

  private:
    size_t target;
  };

  bool isCharDigit(char c)
  {
    return std::isdigit(static_cast< unsigned char >(c)) != 0;
  }

  bool isEvenVertexCount(const shirokov::Polygon& p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOddVertexCount(const shirokov::Polygon& p)
  {
    return p.points.size() % 2 != 0;
  }

  void countEven(std::ostream& out, const shirokov::plg_t& polygons)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isEvenVertexCount) << "\n";
  }

  void countOdd(std::ostream& out, const shirokov::plg_t& polygons)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOddVertexCount) << "\n";
  }

  void countNum(std::ostream& out, const shirokov::plg_t& polygons, size_t num)
  {
    out << std::count_if(polygons.begin(), polygons.end(), NumVertexPredicate{num}) << "\n";
  }
}

void shirokov::count(std::istream& in, std::ostream& out, shirokov::plg_t& polygons)
{
  std::string subCmd;
  if (!(in >> subCmd))
  {
    throw std::logic_error("Missing or unreachable subcommand argument");
  }

  shirokov::IOguard g(out);
  out << std::fixed << std::setprecision(1);

  if (subCmd == "EVEN")
  {
    countEven(out, polygons);
  }
  else if (subCmd == "ODD")
  {
    countOdd(out, polygons);
  }
  else if (std::all_of(subCmd.begin(), subCmd.end(), isCharDigit))
  {
    size_t num = std::stoull(subCmd);
    if (num >= 3)
    {
      countNum(out, polygons, num);
    }
    else
    {
      throw std::logic_error("Invalid number of vertices");
    }
  }
  else
  {
    throw std::logic_error("Unrecognized subcommand");
  }
}
