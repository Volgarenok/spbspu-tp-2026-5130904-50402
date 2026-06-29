#ifndef DATA_STRUCT_IO_HPP
#define DATA_STRUCT_IO_HPP

#include <iosfwd>

#include "dataStruct.hpp"

namespace matveev
{
  std::istream& operator>>(std::istream& in, DataStruct& data);
  std::ostream& operator<<(std::ostream& out, const DataStruct& data);
}

#endif
