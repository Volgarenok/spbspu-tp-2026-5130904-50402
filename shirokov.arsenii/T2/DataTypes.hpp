#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP
#include "DataStruct.hpp"

namespace shirokov
{
  struct UllOctIO
  {
    ull& ref;
  };

  struct UllBinIO
  {
    ull& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  using ull_oct = UllOctIO;
  using ull_bin = UllBinIO;
  using str = StringIO;

  std::istream& operator>>(std::istream&, ull_oct&&);
  std::istream& operator>>(std::istream&, ull_bin&&);
  std::istream& operator>>(std::istream&, str&&);
}

#endif
