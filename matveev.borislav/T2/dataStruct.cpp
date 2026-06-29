#include "dataStruct.hpp"

matveev::DataStruct::DataStruct():
  key1(0),
  key2(0),
  key3()
{}

matveev::DataStruct::DataStruct(
  unsigned long long key1_value,
  char key2_value,
  const std::string& key3_value
):
  key1(key1_value),
  key2(key2_value),
  key3(key3_value)
{}

bool matveev::compareDataStruct(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }

  if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }

  return lhs.key3.size() < rhs.key3.size();
}
