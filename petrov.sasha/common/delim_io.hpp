#ifndef DELIM_IO_HPP
#define DELIM_IO_HPP

#include <ios>
#include <istream>

namespace petrov
{
  struct DelimIO
  {
    char expected;
  };

  inline std::istream &operator>>(std::istream &in, DelimIO &&delim)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '\0';
    in >> c;
    if (in && c != delim.expected) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}

#endif
