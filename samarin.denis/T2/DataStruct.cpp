#include "DataStruct.hpp"

#include <cstddef>
#include <istream>
#include <string>
#include "DelimiterIO.hpp"

namespace {
  using samarin::DelimiterIO;

  struct LabelIO {
    std::string &ref;
  };

  struct UllLiteralIO {
    unsigned long long &ref;
  };

  std::istream &operator>>(std::istream &in, LabelIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    in >> dest.ref;
    return in;
  }
}

std::istream &samarin::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  const std::size_t fieldCount = 3;
  DataStruct input{};
  in >> DelimiterIO{'('};
  for (std::size_t i = 0; i < fieldCount; ++i) {
    in >> DelimiterIO{':'};
    std::string label;
    in >> LabelIO{label};
    in.setstate(std::ios::failbit);
  }
  if (in) {
    dest = input;
  }
  return in;
}
