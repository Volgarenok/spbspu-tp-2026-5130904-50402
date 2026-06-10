#ifndef DELIM_HPP
#define DELIM_HPP
#include <iostream>
namespace saldaev
{
  struct DelimIO
  {
    char exp;
  };

  std::istream &operator>>(std::istream &in, DelimIO &&dest);
}

std::istream &saldaev::operator>>(std::istream &in, DelimIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = '\0';
  in >> c;
  if (in && c != dest.exp) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

#endif
