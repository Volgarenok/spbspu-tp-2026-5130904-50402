#include "KeyIO.hpp"
#include <algorithm>
#include <istream>
#include "DataTypes.hpp"
#include "IOguard.hpp"

std::istream& shirokov::operator>>(std::istream& in, key&& dest)
{
  std::istream::sentry s(in);
  if (!s || dest.used.empty())
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  IOguard g(in);

  DataType currentType = dest.used.back();
  if (std::count(dest.used.begin(), dest.used.end(), currentType) > 1 || currentType == Unknown)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  switch (currentType)
  {
  case UllOct:
    in >> ull_oct{dest.input.key1};
    break;
  case UllBin:
    in >> ull_bin{dest.input.key2};
    break;
  case String:
    in >> str{dest.input.key3};
    break;
  default:
    in.setstate(std::ios::failbit);
    break;
  }
  return in;
}
