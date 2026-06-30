#include "DataStruct.hpp"
#include <iostream>
#include <vector>
#include <IOGuard.hpp>
#include <DelimiterIO.hpp>
#include "LabelIO.hpp"
#include "KeyIO.hpp"

std::istream& novikov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  IOGuard g(in);
  while (true)
  {
    in >> std::noskipws;

    DataStruct input;
    std::vector< DataType > used;
    const std::vector< std::string > possibleLabels{"key1", "key2", "key3"};

    in >> sep{'('} >> sep{':'};
    in >> label{used, possibleLabels} >> key{used, input};
    in >> sep{':'};
    in >> label{used, possibleLabels} >> key{used, input};
    in >> sep{':'};
    in >> label{used, possibleLabels} >> key{used, input};
    in >> sep{':'} >> sep{')'};

    if (in)
    {
      dest = input;
    }

    return in;
  }
}

bool novikov::compare(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }

  double abs_lhs_k2 = std::abs(lhs.key2);
  double abs_rhs_k2 = std::abs(rhs.key2);
  if (abs_lhs_k2 != abs_rhs_k2)
  {
    return abs_lhs_k2 < abs_rhs_k2;
  }

  return lhs.key3.length() < rhs.key3.length();
}

std::ostream& novikov::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }

  IOGuard g(out);
  out << std::fixed << std::setprecision(1);

  out << "(:key1 " << src.key1 << "d";
  out << ":key2 #c(" << src.key2.real() << " " << src.key2.imag() << ")";
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}
