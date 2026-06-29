#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>

namespace matveev
{
  struct DataStruct
  {
    DataStruct();
    DataStruct(unsigned long long key1_value, char key2_value, const std::string& key3_value);

    unsigned long long key1;
    char key2;
    std::string key3;
  };

  bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
