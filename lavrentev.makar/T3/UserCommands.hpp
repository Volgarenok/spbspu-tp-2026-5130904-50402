#ifndef USERCOMMANDS_HPP
#define USERCOMMANDS_HPP
#include "Figures.hpp"

namespace lavrentev
{
  void maxseq(std::istream &is, const std::vector< Polygon > &plgs);
  void intersections(std::istream &is, const std::vector< Polygon > &plgs);
  bool polyInter(const Polygon& A, const Polygon& B);
  bool pInside(const Polygon& poly, const Point& pt);
  size_t rayAccum(size_t acc, const Point&, const std::vector< Point >* pts,
    const Point* pt, size_t* index);
  bool edgeInterAll(size_t idxA, const Polygon* A, const Polygon* B);
  bool checkPair(size_t j, const std::vector< Point >* ptsB, const Point* a1, const Point* a2);
  bool segInter(const Point& p1, const Point& q1, const Point& p2, const Point& q2);
  int orient(const Point& a, const Point& b, const Point& c);
}

#endif
