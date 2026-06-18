#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <string>
#include <iostream>
#include <utility>

namespace muhamadiarov
{
  struct DataStruct
  {
    double key1_;
    std::pair< long long, unsigned long long > key2_;
    std::string key3_;
  };

  struct DoubleIO
  {
    double& data_;
  };

  struct RatIO
  {
    std::pair< long long, unsigned long long >& data_;
  };

  struct StringIO
  {
    std::string& data_;
  };

  struct DelimiterIO
  {
    char exp_;
  };

  struct KeyIO
  {
    Key& key_;
  };

  enum class Key
  {
    KEY1,
    KEY2,
    KEY3
  };

  std::istream& operator>>(std::istream& in, DoubleIO&& d);
  std::istream& operator>>(std::istream& in, RatIO&& r);
  std::istream& operator>>(std::istream& in, StringIO&& s);
  std::istream& operator>>(std::istream& in, DelimiterIO&& c);
  std::istream& operator>>(std::istream& in, KeyIO&& k);

  std::istream& operator>>(std::istream& in, DataStruct&& data);
  std::ostream& operator<<(std::ostream& out, const DataStruct& data);

  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}


#endif
