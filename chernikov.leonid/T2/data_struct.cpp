#include "data_struct.hpp"
#include "io_structs.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

bool chernikov::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  double abs_lhs = std::abs(lhs.key1);
  double abs_rhs = std::abs(rhs.key1);
  if (abs_lhs != abs_rhs)
  {
    return abs_lhs < abs_rhs;
  }

  double val_lhs = static_cast< double >(lhs.key2.first) / static_cast< double >(lhs.key2.second);
  double val_rhs = static_cast< double >(rhs.key2.first) / static_cast< double >(rhs.key2.second);
  if (val_lhs != val_rhs)
  {
    return val_lhs < val_rhs;
  }

  return lhs.key3.length() < rhs.key3.length();
}

std::istream &chernikov::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct tmp;
  bool got1 = false;
  bool got2 = false;
  bool got3 = false;

  in >> DelimIO{'('};

  while (in && (!got1 || !got2 || !got3))
  {
    std::string label;
    in >> label;

    if (label == ":key1" && !got1)
    {
      in >> CmpLspIO{tmp.key1};
      if (in)
      {
        got1 = true;
      }
    } else if (label == ":key2" && !got2)
    {
      in >> RatLspIO{tmp.key2};
      if (in)
      {
        got2 = true;
      }
    } else if (label == ":key3" && !got3)
    {
      in >> StringIO{tmp.key3};
      if (in)
      {
        got3 = true;
      }
    } else
    {
      in.setstate(std::ios::failbit);
    }
  }

  in >> LabelIO{":)"};

  if (in && got1 && got2 && got3)
  {
    dest = tmp;
  } else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream &chernikov::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  out << "(:key1 #c(" << std::fixed << std::setprecision(1) << src.key1.real() << " " << src.key1.imag() << "):"
      << "key2 (:N " << src.key2.first << ":D " << src.key2.second << ":):"
      << "key3 " << std::quoted(src.key3) << ":)";

  return out;
}
