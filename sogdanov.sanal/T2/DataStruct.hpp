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

  struct DelimiterIO
  {
    char exp;
  };

  struct DblLitIO
  {
    double& ref;
  };

  struct SllLitIO
  {
    long long& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  enum class Key
  {
    KEY1,
    KEY2,
    KEY3
  };

  struct KeyIO
  {
    Key& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, DblLitIO&& dest);
  std::istream& operator>>(std::istream& in, SllLitIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, KeyIO&& dest);

  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& src);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
