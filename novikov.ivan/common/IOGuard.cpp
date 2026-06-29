#include "IOGuard.hpp"

novikov::IOGuard::IOGuard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{}

novikov::IOGuard::~IOGuard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
