#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include "commands.hpp"
#include "IOGuard.hpp"

namespace
{
  bool isEvenVertexCount(const novikov::Polygon&);
  bool isOddVertexCount(const novikov::Polygon&);
  void countEven(std::ostream&, const novikov::plg_t&);
  void countOdd(std::ostream&, const novikov::plg_t&);
  void countNum(std::ostream&, const novikov::plg_t&, size_t);

  struct NumVertexPredicate
  {
  public:
    explicit NumVertexPredicate(size_t targetValue):
        target(targetValue)
    {}

    bool operator()(const novikov::Polygon& p) const
    {
      return p.points.size() == target;
    }

  private:
    size_t target;
  };

  bool isEvenVertexCount(const novikov::Polygon& p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOddVertexCount(const novikov::Polygon& p)
  {
    return p.points.size() % 2 != 0;
  }

  void countEven(std::ostream& out, const novikov::plg_t& polygons)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isEvenVertexCount);
  }

  void countOdd(std::ostream& out, const novikov::plg_t& polygons)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOddVertexCount);
  }

  void countNum(std::ostream& out, const novikov::plg_t& polygons, size_t num)
  {
    out << std::count_if(polygons.begin(), polygons.end(), NumVertexPredicate{num});
  }
}

void novikov::count(std::istream& in, std::ostream& out, novikov::plg_t& polygons)
{
  std::string subCmd;
  if (!(in >> subCmd))
  {
    throw std::logic_error("Missing or unreachable subcommand argument");
  }

  novikov::IOGuard g(out);
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
        throw std::logic_error("Invalid subcommand");
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
      throw std::logic_error("Invalid subcommand");
    }
    catch (const std::out_of_range&)
    {
      throw std::logic_error("Invalid number of vertices");
    }
  }
}
