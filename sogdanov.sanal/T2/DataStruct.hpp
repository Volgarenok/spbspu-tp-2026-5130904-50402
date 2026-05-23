#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <string>
#include <iostream>

namespace sogdanov
{
  struct DataStruct
  {
    double key1;
    long long key2;
    std::string key3;
  };

  std::istream &operator>>(std::istream &in, DataStruct &dest);
  std::ostream &operator<<(std::ostream &out, const DataStruct &src);
  bool operator<(const DataStruct &lhs, const DataStruct &rhs);
}

#endif
