#include "utils.hpp"

#include <iomanip>
#include <ioguard.hpp>

namespace petrov
{
  std::istream &operator>>(std::istream &in, DelimIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '\0';
    in >> c;
    if (!in || c != dest.exp) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}

