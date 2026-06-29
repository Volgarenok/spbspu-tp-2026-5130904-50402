#include "io_structs.hpp"
#include <istream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <cctype>

chernikov::StringIO::StringIO(std::string &ref):
  ref_(ref)
{
}

chernikov::DoubleIO::DoubleIO(double &ref):
  ref_(ref)
{
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

std::istream &chernikov::operator>>(std::istream &in, DoubleIO &&value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string token;
  char c = static_cast< char >(in.peek());

  while (in.get(c) && !std::isspace(c) && c != ':')
  {
    token += c;
  }

  if (c == ':')
  {
    in.unget();
  }

  if (token.empty())
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (token.size() >= 2 && (token.substr(token.size() - 2) == "ll" || token.substr(token.size() - 2) == "LL"))
  {
    value.ref_ = static_cast< double >(std::atoll(token.c_str()));
  } else if (token.size() >= 3 && (token.substr(token.size() - 3) == "ull" || token.substr(token.size() - 3) == "ULL"))
  {
    value.ref_ = static_cast< double >(std::strtoull(token.c_str(), nullptr, 10));
  } else if (token.size() >= 1 && token[token.size() - 1] == 'd')
  {
    std::string num_str = token.substr(0, token.size() - 1);
    value.ref_ = std::atof(num_str.c_str());
  } else if (token.size() >= 2 && (token[0] == '0' && (token[1] == 'b' || token[1] == 'B')))
  {
    value.ref_ = static_cast< double >(std::strtoull(token.c_str() + 2, nullptr, 2));
  } else if (token.size() >= 2 && (token[0] == '0' && (token[1] == 'x' || token[1] == 'X')))
  {
    value.ref_ = static_cast< double >(std::strtoull(token.c_str() + 2, nullptr, 16));
  } else if (token.size() >= 1 && token[0] == '0' && token.size() > 1)
  {
    value.ref_ = static_cast< double >(std::strtoull(token.c_str(), nullptr, 8));
  } else if (token.size() >= 3 && token[0] == '\'' && token[token.size() - 1] == '\'')
  {
    if (token.size() == 3)
    {
      value.ref_ = static_cast< double >(token[1]);
    } else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  } else if (token.size() >= 4 && token[0] == '#' && token[1] == 'c' && token[2] == '(')
  {
    std::string complex_str = token.substr(3, token.size() - 4);
    size_t space_pos = complex_str.find(' ');
    if (space_pos != std::string::npos)
    {
      double real = std::atof(complex_str.substr(0, space_pos).c_str());
      double imag = std::atof(complex_str.substr(space_pos + 1).c_str());
      value.ref_ = std::sqrt(real * real + imag * imag);
    } else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  } else if (token.size() >= 7 && token[0] == '(' && token[1] == ':' && token[2] == 'N')
  {
    size_t n_end = token.find(":D");
    size_t d_end = token.find(":)", n_end + 2);
    if (n_end != std::string::npos && d_end != std::string::npos)
    {
      long long num = std::atoll(token.substr(3, n_end - 3).c_str());
      unsigned long long den = std::strtoull(token.substr(n_end + 2, d_end - n_end - 2).c_str(), nullptr, 10);
      if (den != 0)
      {
        value.ref_ = static_cast< double >(num) / static_cast< double >(den);
      } else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    } else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  } else
  {
    value.ref_ = std::atof(token.c_str());
  }

  return in;
}
