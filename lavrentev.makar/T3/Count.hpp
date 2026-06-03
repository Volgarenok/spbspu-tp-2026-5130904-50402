#ifndef COUNT_HPP
#define COUNT_HPP
#include <iostream>
#include "Figures.hpp"

namespace lavrentev
{
  void count(std::istream& is, const std::vector< Polygon >& plgs);
  void countEven(std::istream&, const std::vector< Polygon >& plgs);
  void countOdd(std::istream&, const std::vector< Polygon >& plgs);
  void countVrtxs(const std::vector< Polygon >& plgs, size_t n); //TODO
}

#endif
