#ifndef PARSER_HPP
#define PARSER_HPP
#include <istream>
#include "commands.hpp"

namespace novikov
{
  struct IOGuard
  {
    explicit IOGuard(std::basic_ios< char >&);
    ~IOGuard();

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct PolygonIO
  {
    novikov::Polygon& p;
  };

  std::istream& operator>>(std::istream&, Point&);
  std::istream& operator>>(std::istream&, DelimiterIO&&);
  std::istream& operator>>(std::istream&, PolygonIO&&);
  std::istream& operator>>(std::istream&, Polygon&);
  plg_t parse(std::istream&);
}

#endif
