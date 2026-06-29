#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>

namespace afanasev
{
  struct DelimIO
  {
    char expected;
  };

  class IOguard
  {
  public:
    IOguard(std::ios_base & stream) : stream_(stream), flags_(stream.flags()) {}
    ~IOguard() { stream_.flags(flags_); }
  private:
    std::ios_base & stream_;
    std::ios::fmtflags flags_;
  };

  std::istream & operator>>(std::istream & in, DelimIO && delim);
}

#endif
