#ifndef DELIM_HPP
#define DELIM_HPP

#include <istream>

namespace chernikov {
  struct DelimIO
  {
    char exp;
  };

  std::istream &operator>>(std::istream &in, DelimIO &&dest);
}
#endif
