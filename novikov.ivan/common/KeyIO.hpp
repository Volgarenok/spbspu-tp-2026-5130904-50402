#ifndef KEY_IO_HPP
#define KEY_IO_HPP
#include <vector>
#include "DataStruct.hpp"

namespace novikov
{
  struct KeyIO
  {
    const std::vector< DataType >& used;
    DataStruct& input;
  };

  using key = KeyIO;

  std::istream& operator>>(std::istream&, key&&);
}

#endif
