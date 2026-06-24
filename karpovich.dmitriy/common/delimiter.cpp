#include "delimiter.hpp"

std::istream &karpovich::operator>>(std::istream &in, DelimIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = '\0';
  in >> c;
  if (dest.exp2 == '\0') {
    if (in && c != dest.exp1) {
      in.setstate(std::ios::failbit);
    }
  } else {
    if (in && c != dest.exp1 && c != dest.exp2) {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}
