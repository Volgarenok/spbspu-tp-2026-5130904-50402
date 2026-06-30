#include "ioguard.hpp"

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
