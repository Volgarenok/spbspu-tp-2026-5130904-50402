#ifndef USERCOMMANDS_HPP
#define USERCOMMANDS_HPP
#include "Figures.hpp"

namespace lavrentev
{
  void maxseq(std::istream &is, const std::vector< Polygon > &plgs);
  void intersections(std::istream &is, const std::vector< Polygon > &plgs);
}

#endif
