#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include "IOguard.hpp"
#include "commands.hpp"

namespace
{
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
    out << std::count_if(polygons.begin(), polygons.end(), isEvenVertexCount);
  }

  void countOdd(std::ostream& out, const shirokov::plg_t& polygons)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOddVertexCount);
  }

  void countNum(std::ostream& out, const shirokov::plg_t& polygons, size_t num)
  {
    out << std::count_if(polygons.begin(), polygons.end(), NumVertexPredicate{num});
  }
}

void shirokov::count(std::istream& in, std::ostream& out, plg_t& polygons)
{
  std::string subCmd;
  if (!(in >> subCmd))
  {
    throw std::logic_error("Missing or unreachable subcommand argument");
  }

  IOguard g(out);
  out << std::fixed << std::setprecision(1);

  using subCmd_t = void (*)(std::ostream&, const plg_t&);
  std::unordered_map< std::string, subCmd_t > subCmds;
  subCmds["EVEN"] = countEven;
  subCmds["ODD"] = countOdd;

  if (subCmds.count(subCmd))
  {
    subCmds[subCmd](out, polygons);
  }
  else
  {
    try
    {
      size_t idx = 0;
      size_t num = std::stoull(subCmd, &idx);

      if (idx != subCmd.length())
      {
        throw std::logic_error("Unrecognized subcommand");
      }

      if (num >= 3)
      {
        countNum(out, polygons, num);
      }
      else
      {
        throw std::logic_error("Invalid number of vertices");
      }
    }
    catch (const std::invalid_argument&)
    {
      throw std::logic_error("Unrecognized subcommand");
    }
    catch (const std::out_of_range&)
    {
      throw std::logic_error("Invalid number of vertices");
    }
  }
}
