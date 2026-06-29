#include "DelimiterIO.hpp"
#include "IOguard.hpp"

std::istream& shirokov::operator>>(std::istream& in, sep&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  IOguard g(in);
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

std::istream& shirokov::operator>>(std::istream& in, uncase_sep&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  IOguard g(in);
  char c = 0;
  if (in.get(c))
  {
    if (std::toupper(static_cast< unsigned char >(c)) != std::toupper(static_cast< unsigned char >(dest.exp))
        && (std::tolower(static_cast< unsigned char >(c)) != std::tolower(static_cast< unsigned char >(dest.exp))))
    {
      in.putback(c);
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}
