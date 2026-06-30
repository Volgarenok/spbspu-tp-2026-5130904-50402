#ifndef DELIM_HPP
#define DELIM_HPP
#include <iostream>

namespace chernikov {
  struct DelimIO
  {
    char exp;
  };

  std::istream &operator>>(std::istream &in, DelimIO &&dest);
}
#endif
