#include "DelimiterIO.hpp"
#include "IOGuard.hpp"

std::istream& novikov::operator>>(std::istream& in, sep&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  IOGuard g(in);
  char c = 0;
  if (in.get(c))
  {
    if (c != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::istream& novikov::operator>>(std::istream& in, uncase_sep&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  IOGuard g(in);
  char c = 0;
  if (in.get(c))
  {
    if (std::toupper(static_cast< unsigned char >(c)) != std::toupper(static_cast< unsigned char >(dest.exp)))
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}
