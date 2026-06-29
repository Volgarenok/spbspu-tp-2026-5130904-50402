#ifndef LABEL_IO_HPP
#define LABEL_IO_HPP
#include <vector>
#include "DataStruct.hpp"

namespace shirokov
{
  struct LabelIO
  {
    std::vector< DataType >& used;
    const std::vector< std::string >& possibleLabels;
  };

  using label = LabelIO;
  std::istream& operator>>(std::istream&, label&&);
}

#endif
