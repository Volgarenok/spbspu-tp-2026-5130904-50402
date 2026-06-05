#include "IoUtils.hpp"

namespace sogdanov {

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  IoGuard::IoGuard(std::basic_ios< char >& s):
    s_(s),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  IoGuard::~IoGuard()
  {
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }

}
