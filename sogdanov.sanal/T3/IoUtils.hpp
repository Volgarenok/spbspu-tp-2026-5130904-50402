#ifndef IO_UTILS_HPP
#define IO_UTILS_HPP

#include <istream>
#include <ios>

namespace sogdanov {

  struct DelimiterIO {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);

}

#endif
