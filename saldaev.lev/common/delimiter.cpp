#include "delimiter.hpp"

std::istream &saldaev::operator>>(std::istream &in, DelimIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  char c = '\0';
  in >> c;

  if (dest.exp2 == '\0') {
    if (c != dest.exp1) {
      in.setstate(std::ios::failbit);
    }
  } else {
    if (c != dest.exp1 && c != dest.exp2) {
      in.setstate(std::ios::failbit);
    } else if (dest.matchedSecond) {
      *dest.matchedSecond = (c == dest.exp2);
    }
  }

  return in;
}
