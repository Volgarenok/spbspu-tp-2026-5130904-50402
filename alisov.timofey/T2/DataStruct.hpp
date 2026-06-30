#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <string>

namespace alisov
{
  struct DataStruct
  {
    double key1;
    double key2;
    std::string key3;
  };

  std::istream &operator>>(std::istream &in, DataStruct &dest);
  std::ostream &operator<<(std::ostream &out, const DataStruct &src);

  bool operator<(const DataStruct &lhs, const DataStruct &rhs);
}

#endif
