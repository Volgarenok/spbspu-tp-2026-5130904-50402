#include "delimiterio.hpp"

std::istream& muhamadiarov::operator>>(std::istream& in, DelimiterIO&& data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = ' ';
  in >> std::ws;
  in >> c;
  if (c != data.exc)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
