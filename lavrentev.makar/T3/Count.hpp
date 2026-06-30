#ifndef COUNT_HPP
#define COUNT_HPP
#include <iostream>
#include <istream>
#include <algorithm>
#include "Figures.hpp"

namespace lavrentev
{
  void count(std::istream &is, const std::vector< Polygon > &plgs);
  void countEven(std::istream &, const std::vector< Polygon > &plgs);
  void countOdd(std::istream &, const std::vector< Polygon > &plgs);
  void countVrtxs(const std::vector< Polygon > &plgs, size_t n);

  template < typename Predicate >
  void countIf(const std::vector< Polygon > &plgs, Predicate pred);
}

template < typename Predicate >
void lavrentev::countIf(const std::vector< lavrentev::Polygon > &plgs, Predicate pred)
{
  std::cout << std::count_if(plgs.begin(), plgs.end(), pred);
}

#endif
