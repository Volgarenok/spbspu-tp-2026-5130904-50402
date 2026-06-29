#include "io_structs.hpp"
#include <istream>
#include <string>
#include <cstdlib>

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

  std::string token;
  char c;

  while (in.get(c) && c != ':')
  {
    token += c;
  }

  if (c == ':')
  {
    in.unget();
  }

  if (token.size() < 5 || token[0] != '#' || token[1] != 'c' || token[2] != '(' || token[token.size() - 1] != ')')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::string inner = token.substr(3, token.size() - 4);
  size_t space = inner.find(' ');
  if (space == std::string::npos)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  double real = std::atof(inner.substr(0, space).c_str());
  double imag = std::atof(inner.substr(space + 1).c_str());
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

  while (in.get(c) && c != ':')
  {
    token += c;
  }

  if (c == ':')
  {
    in.unget();
  }

  if (token.size() < 7 || token[0] != '(' || token[1] != ':' || token[2] != 'N')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  size_t n_start = 3;
  size_t n_end = token.find(":D");
  if (n_end == std::string::npos)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  size_t d_start = n_end + 2;
  size_t d_end = token.find(":)", d_start);
  if (d_end == std::string::npos)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  long long num = std::atoll(token.substr(n_start, n_end - n_start).c_str());
  unsigned long long den = std::strtoull(token.substr(d_start, d_end - d_start).c_str(), nullptr, 10);

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
