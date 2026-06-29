#include <ioGuard.hpp>

matveev::IOGuard::IOGuard(std::basic_ios< char >& stream):
  stream_(stream),
  flags_(stream.flags()),
  fill_(stream.fill()),
  precision_(stream.precision())
{}

matveev::IOGuard::~IOGuard()
{
  stream_.flags(flags_);
  stream_.fill(fill_);
  stream_.precision(precision_);
}
