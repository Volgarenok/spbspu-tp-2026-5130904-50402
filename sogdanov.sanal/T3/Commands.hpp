#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <vector>
#include "Polygon.hpp"

namespace sogdanov {

  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void perms(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void maxseq(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);

  void context(std::istream& in, std::ostream& out, std::vector< std::vector< Polygon > >& contexts);
  void popcontext(std::istream& in, std::ostream& out, std::vector< std::vector< Polygon > >& contexts);
  void level(std::istream& in, std::ostream& out, const std::vector< std::vector< Polygon > >& contexts);
}

#endif
