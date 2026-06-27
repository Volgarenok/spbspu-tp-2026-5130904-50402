#include "Polygon.hpp"

#include <istream>
#include "DelimiterIO.hpp"

std::istream &samarin::operator>>(std::istream &in, Point &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  Point input{0, 0};
  in >> DelimiterIO{'('} >> input.x >> DelimiterIO{';'} >> input.y >> DelimiterIO{')'};
  if (in) {
    dest = input;
  }
  return in;
}
