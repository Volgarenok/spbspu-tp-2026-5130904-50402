#ifndef AREA_HPP
#define AREA_HPP
#include "Figures.hpp"
#include <numeric>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <iterator>

namespace lavrentev
{
  void area(std::istream &is, const std::vector< Polygon > &plgs);
  void areaEven(std::istream &, const std::vector< Polygon > &plgs);
  void areaOdd(std::istream &, const std::vector< Polygon > &plgs);
  void areaMean(std::istream &, const std::vector< Polygon > &plgs);
  void areaVrtxs(const std::vector< Polygon > &plgs, size_t n);

  template < typename Predicate >
  void areaIf(const std::vector< Polygon > &plgs, Predicate pred);
}

template < typename Predicate >
void lavrentev::areaIf(const std::vector< lavrentev::Polygon > &plgs, Predicate pred)
{
  using namespace std::placeholders;

  std::vector< lavrentev::Polygon > filtered;
  std::copy_if(plgs.begin(), plgs.end(), std::back_inserter(filtered), pred);

  std::vector< float > areas(filtered.size());
  std::transform(
    filtered.begin(),
    filtered.end(),
    areas.begin(),
    std::bind(&lavrentev::Polygon::getArea, _1)
  );

  float total = std::accumulate(areas.begin(), areas.end(), 0.0f);
  std::cout << std::fixed << std::setprecision(1) << total << "\n";
}

#endif
