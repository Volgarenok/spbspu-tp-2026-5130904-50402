#include "io_structs.hpp"
#include <istream>
#include <string>
#include <cstdlib>
#include <cmath>

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

static double parseDoubleToken(const std::string &token)
{
  if (token.empty())
  {
    return 0.0;
  }

  if (token[0] == '\'' && token.size() >= 3 && token[token.size() - 1] == '\'')
  {
    return static_cast< double >(token[1]);
  }

  if (token.size() >= 2 && token[0] == '"' && token[token.size() - 1] == '"')
  {
    std::string inner = token.substr(1, token.size() - 2);
    return static_cast< double >(inner.length() > 0 ? static_cast< unsigned char >(inner[0]) : 0);
  }

  if (token.size() >= 2 && (token.substr(token.size() - 2) == "ll" || token.substr(token.size() - 2) == "LL"))
  {
    return static_cast< double >(std::atoll(token.c_str()));
  }

  if (token.size() >= 3 && (token.substr(token.size() - 3) == "ull" || token.substr(token.size() - 3) == "ULL"))
  {
    return static_cast< double >(std::strtoull(token.c_str(), nullptr, 10));
  }

  if (token.size() >= 1 && token[token.size() - 1] == 'd')
  {
    return std::atof(token.substr(0, token.size() - 1).c_str());
  }

  if (token.size() >= 2 && token[0] == '0' && (token[1] == 'b' || token[1] == 'B'))
  {
    return static_cast< double >(std::strtoull(token.c_str() + 2, nullptr, 2));
  }

  if (token.size() >= 2 && token[0] == '0' && (token[1] == 'x' || token[1] == 'X'))
  {
    return static_cast< double >(std::strtoull(token.c_str() + 2, nullptr, 16));
  }

  if (token.size() > 1 && token[0] == '0' && token[1] != 'b' && token[1] != 'B' && token[1] != 'x' && token[1] != 'X')
  {
    return static_cast< double >(std::strtoull(token.c_str(), nullptr, 8));
  }

  return std::atof(token.c_str());
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

  if (token.size() >= 5 && token[0] == '#' && token[1] == 'c' && token[2] == '(' && token[token.size() - 1] == ')')
  {
    std::string inner = token.substr(3, token.size() - 4);
    size_t space = inner.find(' ');
    if (space != std::string::npos)
    {
      double real = std::atof(inner.substr(0, space).c_str());
      double imag = std::atof(inner.substr(space + 1).c_str());
      value.ref_ = std::complex< double >(real, imag);
      return in;
    }
  }

  double num = parseDoubleToken(token);
  value.ref_ = std::complex< double >(num, 0.0);
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

  if (token.size() >= 7 && token[0] == '(' && token[1] == ':' && token[2] == 'N')
  {
    size_t n_start = 3;
    size_t n_end = token.find(":D");
    if (n_end != std::string::npos)
    {
      size_t d_start = n_end + 2;
      size_t d_end = token.find(":)", d_start);
      if (d_end != std::string::npos)
      {
        long long num = std::atoll(token.substr(n_start, n_end - n_start).c_str());
        unsigned long long den = std::strtoull(token.substr(d_start, d_end - d_start).c_str(), nullptr, 10);
        if (den != 0)
        {
          value.ref_ = std::make_pair(num, den);
          return in;
        }
      }
    }
  }

  double num = parseDoubleToken(token);
  long long numerator = static_cast< long long >(num);
  value.ref_ = std::make_pair(numerator, 1ULL);
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
