#include "IoUtils.hpp"

std::istream& sogdanov::operator>>(std::istream& in, sogdanov::DelimiterIO&& dest)
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
