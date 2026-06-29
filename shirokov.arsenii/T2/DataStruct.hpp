#ifndef DATA_STRUCT
#define DATA_STRUCT
#include <string>

namespace shirokov
{
  using ull = unsigned long long;

  struct DataStruct
  {
    ull key1;
    ull key2;
    std::string key3;
  };

  enum DataType
  {
    UllOct,
    UllBin,
    String,
    Unknown
  };

  std::istream& operator>>(std::istream&, DataStruct&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);
  bool compare(const DataStruct&, const DataStruct&);
}

#endif
