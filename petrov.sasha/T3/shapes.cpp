#include "shapes.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <numeric>
#include <utility>

namespace petrov
{
  bool operator==(const Point& lhs, const Point& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }
  
  IOguard::IOguard(std::istream& stream):
    stream_(stream),
    flags_(stream.flags())
  {}

  IOguard::~IOguard()
  {
    stream_.flags(flags_);
  }

}