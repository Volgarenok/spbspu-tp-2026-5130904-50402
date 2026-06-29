#include "StreamGuard.hpp"

samarin::StreamGuard::StreamGuard(std::basic_ios< char > &stream):
  stream_(stream),
  flags_(stream.flags()),
  precision_(stream.precision()),
  fill_(stream.fill())
{}

samarin::StreamGuard::~StreamGuard()
{
  stream_.flags(flags_);
  stream_.precision(precision_);
  stream_.fill(fill_);
}
