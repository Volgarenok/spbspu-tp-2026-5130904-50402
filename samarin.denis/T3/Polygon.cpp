#include "Polygon.hpp"

#include <algorithm>
#include <cstddef>
#include <istream>
#include <iterator>
#include <vector>
#include "DelimiterIO.hpp"

namespace {
  bool restOfLineIsBlank(std::istream &in)
  {
    int next = in.peek();
    while (next == ' ' || next == '\t') {
      in.get();
      next = in.peek();
    }
    return (next == '\n') || (next == std::char_traits< char >::eof());
  }
}

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

std::istream &samarin::operator>>(std::istream &in, Polygon &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::size_t count = 0;
  in >> count;
  if (!in || (count < 3)) {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > input;
  input.reserve(count);
  std::copy_n(std::istream_iterator< Point >(in), count, std::back_inserter(input));
  if (in && (input.size() == count) && restOfLineIsBlank(in)) {
    dest.points = input;
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}
