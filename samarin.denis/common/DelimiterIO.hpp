#ifndef DELIMITER_IO_HPP
#define DELIMITER_IO_HPP

#include <iosfwd>

namespace samarin {
  struct DelimiterIO {
    char expected;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
}

#endif
