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
void max(std::istream& is, std::vector< Polygon > plgs);
void min(std::istream& is, std::vector< Polygon > plgs);
void count(std::istream& is, std::vector< Polygon > plgs);
void maxseq(std::istream& is, std::vector< Polygon > plgs);
void intersections(std::istream& is, std::vector< Polygon > plgs);
