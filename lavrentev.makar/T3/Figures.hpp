#include <cstddef>
#include <vector>

struct Point
{
  int x, y;
};

struct Polygon
{
  std::vector< Point > points;
};

void area(std::istream& is, std::vector< Polygon > plgs);
void areaEven(std::vector< Polygon > plgs);
void areaOdd(std::vector< Polygon > plgs);
void areaMean(std::vector< Polygon > plgs);
void areaVrtxs(std::vector< Polygon > plgs, size_t n);
void max(std::istream& is, std::vector< Polygon > plgs);
void min(std::istream& is, std::vector< Polygon > plgs);
void count(std::istream& is, std::vector< Polygon > plgs);
void maxseq(std::istream& is, std::vector< Polygon > plgs);
void intersections(std::istream& is, std::vector< Polygon > plgs);
