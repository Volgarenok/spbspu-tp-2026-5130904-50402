#include "DelimiterIO.hpp"

#include <istream>

std::istream &samarin::operator>>(std::istream &in, DelimiterIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char received = 0;
  in >> received;
  if (in && (received != dest.expected)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}
