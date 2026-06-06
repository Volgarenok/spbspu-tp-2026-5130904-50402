#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <complex>
#include <utility>
#include <cmath>
#include <cctype>

namespace chernikov {

  struct DataStruct
  {
    std::complex< double > key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };

  double key2_to_double(const std::pair< long long, unsigned long long > &p)
  {
    return static_cast< double >(p.first) / static_cast< double >(p.second);
  }

  bool compare_data(const DataStruct &a, const DataStruct &b)
  {
    double abs_a = std::abs(a.key1);
    double abs_b = std::abs(b.key1);

    if (abs_a != abs_b)
    {
      return abs_a < abs_b;
    }

    double val_a = key2_to_double(a.key2);
    double val_b = key2_to_double(b.key2);

    if (val_a != val_b)
    {
      return val_a < val_b;
    }

    return a.key3.length() < b.key3.length();
  }

  std::string read_string(std::istream &in)
  {
    std::string result;
    char c;

    if (!(in >> c) || c != '"')
    {
      in.setstate(std::ios::failbit);
      return "";
    }

    while (in.get(c))
    {
      if (c == '"')
      {
        return result;
      }
      if (c == '\\')
      {
        if (in.get(c))
        {
          result += c;
        }
      } else
      {
        result += c;
      }
    }

    in.setstate(std::ios::failbit);
    return "";
  }
}

int main()
{
  return 0;
}