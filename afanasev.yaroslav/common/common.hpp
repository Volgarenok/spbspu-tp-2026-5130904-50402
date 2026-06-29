#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>

namespace afanasev
{
  struct DelimIO
  {
    char expected;
  };

  std::istream & operator>>(std::istream & in, DelimIO && delim);
}

#endif
