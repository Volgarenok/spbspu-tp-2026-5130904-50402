#include "io_structs.hpp"
#include <istream>
#include <string>
#include <cstdlib>
#include <cctype>

chernikov::StringIO::StringIO(std::string &ref):
  ref_(ref)
{
}

chernikov::ValueIO::ValueIO(double &ref):
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

std::istream &chernikov::operator>>(std::istream &in, ValueIO &&value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char first = static_cast< char >(in.peek());

  if (first == '"')
  {
    std::string str;
    char c;
    if (!(in >> c) || c != '"')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    while (in.get(c))
    {
      if (c == '"')
      {
        value.ref_ = static_cast< double >(str.length() > 0 ? static_cast< unsigned char >(str[0]) : 0);
        return in;
      }
      if (c == '\\')
      {
        if (in.get(c))
        {
          str += c;
        } else
        {
          in.setstate(std::ios::failbit);
          return in;
        }
      } else
      {
        str += c;
      }
    }
    in.setstate(std::ios::failbit);
    return in;
  }

  if (first == '\'')
  {
    char c;
    if (!(in >> c) || c != '\'')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (!(in.get(c)))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    char ch = c;
    if (!(in >> c) || c != '\'')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    value.ref_ = static_cast< double >(ch);
    return in;
  }

  if (first == '#')
  {
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
        value.ref_ = real;
        return in;
      }
    }
    in.setstate(std::ios::failbit);
    return in;
  }

  if (first == '(')
  {
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

    if (token.size() >= 3 && token[0] == '(' && token[1] == ':' && token[2] == 'N')
    {
      size_t n_start = 3;
      size_t n_end = token.find(":D");
      if (n_end != std::string::npos)
      {
        std::string num_str = token.substr(n_start, n_end - n_start);
        long long num = std::atoll(num_str.c_str());

        size_t d_start = n_end + 2;
        size_t d_end = token.find(":)", d_start);
        if (d_end != std::string::npos)
        {
          std::string den_str = token.substr(d_start, d_end - d_start);
          unsigned long long den = std::strtoull(den_str.c_str(), nullptr, 10);
          if (den != 0)
          {
            value.ref_ = static_cast< double >(num) / static_cast< double >(den);
            return in;
          }
        }
      }
    }
    in.setstate(std::ios::failbit);
    return in;
  }

  std::string token;
  char c;

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
  } else if (token.size() >= 2 && token[0] == '0' && (token[1] == 'b' || token[1] == 'B'))
  {
    value.ref_ = static_cast< double >(std::strtoull(token.c_str() + 2, nullptr, 2));
  } else if (token.size() >= 2 && token[0] == '0' && (token[1] == 'x' || token[1] == 'X'))
  {
    value.ref_ = static_cast< double >(std::strtoull(token.c_str() + 2, nullptr, 16));
  } else if (token.size() > 1 && token[0] == '0' && token[1] != 'b' && token[1] != 'B' && token[1] != 'x'
             && token[1] != 'X')
  {
    value.ref_ = static_cast< double >(std::strtoull(token.c_str(), nullptr, 8));
  } else
  {
    value.ref_ = std::atof(token.c_str());
  }

  return in;
}
