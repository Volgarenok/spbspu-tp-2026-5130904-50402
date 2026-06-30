#ifndef PARSER_HPP
#define PARSER_HPP
#include <istream>
#include <string>
#include "commands.hpp"

namespace novikov
{
  struct PolygonIO
  {
    novikov::Polygon& p;
  };

  struct Line
  {
    std::string lineText;
  };

  struct PolygonLineInserter
  {
  public:
    explicit PolygonLineInserter(plg_t&);
    PolygonLineInserter& operator=(const Line&);
    PolygonLineInserter& operator*();
    PolygonLineInserter& operator++();
    PolygonLineInserter& operator++(int);

  private:
    plg_t& container;
  };

  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, PolygonIO&&);
  std::istream& operator>>(std::istream&, Polygon&);
  std::istream& operator>>(std::istream&, Line&);
  plg_t parse(std::istream&);
}

#endif
