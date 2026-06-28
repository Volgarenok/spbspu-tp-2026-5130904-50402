#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <cstddef>
#include <functional>
#include <iosfwd>
#include <string>
#include <vector>

#include "shapes.hpp"

namespace petrov
{
  namespace detail
  {
    struct Bounds
    {
      int minX;
      int minY;
      int maxX;
      int maxY;
    };

    bool isEven(const Polygon &p);
    bool isOdd(const Polygon &p);
    bool hasVertexCount(const Polygon &p, std::size_t n);
    bool isSpaceChar(char c);

    bool areaLess(const Polygon &a, const Polygon &b);
    bool vertexLess(const Polygon &a, const Polygon &b);

    Bounds getPolygonBounds(const Polygon &poly);
    bool compareByMinX(const Bounds &a, const Bounds &b);
    bool compareByMinY(const Bounds &a, const Bounds &b);
    bool compareByMaxX(const Bounds &a, const Bounds &b);
    bool compareByMaxY(const Bounds &a, const Bounds &b);

    bool isPointInsideFrame(const Point &p, int minX, int minY, int maxX, int maxY);

    std::size_t convertVertexCount(const std::string &param);
    Polygon readPolygonParam(std::istream &in);

    void printFilteredSum(
      std::ostream &out,
      const std::vector< Polygon > &polygons,
      std::function< bool(const Polygon &) > pred
    );
    void printMeanArea(std::ostream &out, const std::vector< Polygon > &polygons);
    void printMaxArea(std::ostream &out, const std::vector< Polygon > &polygons);
    void printMaxVertexes(std::ostream &out, const std::vector< Polygon > &polygons);
    void printMinArea(std::ostream &out, const std::vector< Polygon > &polygons);
    void printMinVertexes(std::ostream &out, const std::vector< Polygon > &polygons);
    void printEvenCount(std::ostream &out, const std::vector< Polygon > &polygons);
    void printOddCount(std::ostream &out, const std::vector< Polygon > &polygons);
    void printVertexCount(std::ostream &out, const std::vector< Polygon > &polygons, std::size_t count);
  }

  void area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void perms(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void inframe(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
}

#endif
