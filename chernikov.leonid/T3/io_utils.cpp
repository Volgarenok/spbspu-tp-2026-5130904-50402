#include "io_utils.hpp"
#include <istream>

chernikov::IOguard::IOguard(std::basic_ios< char > &stream):
  stream_(stream),
  flags_(stream.flags()),
  precision_(stream.precision())
{
}

chernikov::IOguard::~IOguard()
{
  stream_.flags(flags_);
  stream_.precision(precision_);
}

std::istream &chernikov::operator>>(std::istream &in, DelimIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '\0';
  in >> c;
  if (!in || c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
