#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <complex>
#include <ios>
#include <iostream>
#include <string>
#include <IOGuard.hpp>

namespace novikov
{
  struct DataStruct
  {
    double key1;
    std::complex< double > key2;
    std::string key3;
  };

  enum DataType
  {
    DblLit,
    CmpLsp,
    String,
    Unknown
  };

  std::istream& operator>>(std::istream&, DataStruct&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);
  bool compare(const DataStruct&, const DataStruct&);
}

#endif
