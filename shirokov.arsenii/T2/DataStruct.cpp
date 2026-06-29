#include "DataStruct.hpp"
#include <istream>
#include <limits>
#include <ostream>
#include <vector>
#include "DelimiterIO.hpp"
#include "IOguard.hpp"
#include "KeyIO.hpp"
#include "LabelIO.hpp"

bool shirokov::compare(const DataStruct& lhs, const DataStruct& rhs)
{
  bool c = lhs.key1 < rhs.key1;
  c = c || (lhs.key1 == rhs.key1 && lhs.key2 < rhs.key2);
  c = c || (lhs.key1 == rhs.key1 && lhs.key2 == rhs.key2 && lhs.key3.length() < rhs.key3.length());
  return c;
}

std::ostream& shirokov::operator<<(std::ostream& out, const DataStruct& src)
{
  IOguard g(out);
  std::string bin_str;
  ull temp = src.key2;
  if (temp == 0)
  {
    bin_str = "0";
  }
  while (temp > 0)
  {
    bin_str = (temp % 2 == 0 ? "0" : "1") + bin_str;
    temp /= 2;
  }

  out << "(:key1 0" << std::oct << src.key1 << std::dec;
  out << ":key2 0b" << bin_str;
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}

std::istream& shirokov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  IOguard g(in);
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
      return in;
    }

    in.clear();
    auto toIgnore = std::numeric_limits< std::streamsize >::max();
    in.ignore(toIgnore, '\n');
  }
}
