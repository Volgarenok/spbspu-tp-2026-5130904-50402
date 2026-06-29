#include "common.hpp"
#include <iostream>

namespace afanasev
{
  std::istream & operator>>(std::istream & in, DelimIO && delim)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }

    char c = 0;
    in >> c;
    if (in && c != delim.expected)
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }
}
