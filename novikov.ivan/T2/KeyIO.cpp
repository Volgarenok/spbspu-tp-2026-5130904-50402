#include "KeyIO.hpp"
#include <algorithm>
#include <istream>
#include <IOGuard.hpp>
#include "DataTypes.hpp"

std::istream& novikov::operator>>(std::istream& in, key&& dest)
{
  std::istream::sentry s(in);
  if (!s || dest.used.empty())
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  IOGuard g(in);

  DataType currentType = dest.used.back();

  int count = 0;
  for (auto t : dest.used) {
    if (t == currentType) {
      count++;
    }
  }
  if (count > 1 || currentType == Unknown)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  switch (currentType)
  {
    case DblLit:
      in >> dbl_lit{dest.input.key1};
      break;
    case CmpLsp:
      in >> cmp_lsp{dest.input.key2};
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
