#include "io_structs.hpp"
#include <istream>
#include <cctype>
#include <cstdlib>
#include <cstring>

chernikov::ComplexIO::ComplexIO(std::complex< double > &ref):
  ref_(ref)
{
}

chernikov::RationalIO::RationalIO(std::pair< long long, unsigned long long > &ref):
  ref_(ref)
{
}

chernikov::StringIO::StringIO(std::string &ref):
  ref_(ref)
{
}

std::istream &chernikov::operator>>(std::istream &in, ComplexIO &&value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c;
  if (!(in >> c) || c != '#')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> c) || c != 'c')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> c) || c != '(')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  double real = 0.0;
  double imag = 0.0;

  if (!(in >> real))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> imag))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!(in >> c) || c != ')')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  value.ref_ = std::complex< double >(real, imag);
  return in;
}

std::istream &chernikov::operator>>(std::istream &in, RationalIO &&value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string token;
  char c;

  if (!(in >> c) || c != '(')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!(in >> c) || c != ':')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!(in >> c) || c != 'N')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  long long num = 0;
  if (!(in >> num))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!(in >> c) || c != ':')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!(in >> c) || c != 'D')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  unsigned long long den = 0;
  if (!(in >> den))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!(in >> c) || c != ':')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (!(in >> c) || c != ')')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (den == 0)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  value.ref_ = std::make_pair(num, den);
  return in;
}

std::istream &chernikov::operator>>(std::istream &in, StringIO &&value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c;
  if (!(in >> c) || c != '"')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  value.ref_.clear();
  while (in.get(c))
  {
    if (c == '"')
    {
      return in;
    }
    if (c == '\\')
    {
      if (in.get(c))
      {
        value.ref_ += c;
      } else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    } else
    {
      value.ref_ += c;
    }
  }

  in.setstate(std::ios::failbit);
  return in;
}
