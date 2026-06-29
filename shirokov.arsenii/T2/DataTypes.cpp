#include "DataTypes.hpp"
#include <istream>
#include "DelimiterIO.hpp"

std::istream& shirokov::operator>>(std::istream& in, ull_oct&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> std::oct >> dest.ref;
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, ull_bin&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }

  in >> sep{'0'} >> uncase_sep{'b'};
  if (!in)
  {
    return in;
  }

  std::string bin_str;
  char c = 0;
  while (in.get(c))
  {
    if (c == '0' || c == '1')
    {
      bin_str += c;
    }
    else
    {
      in.putback(c);
      break;
    }
  }

  if (bin_str.empty())
  {
    in.setstate(std::ios_base::failbit);
  }
  else
  {
    try
    {
      dest.ref = std::stoull(bin_str, nullptr, 2);
    }
    catch (...)
    {
      in.setstate(std::ios_base::failbit);
    }
  }
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, str&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> sep{'"'};
  if (!in)
  {
    return in;
  }
  return std::getline(in, dest.ref, '"');
}
