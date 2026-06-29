#ifndef DELIMITERIO_HPP
#define DELIMITERIO_HPP
#include <iostream>

namespace muhamadiarov
{
  struct DelimiterIO
  {
    char exc;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& data);
}
#endif
