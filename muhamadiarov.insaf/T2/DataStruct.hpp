#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <string>
#include <iostream>
#include <utility>
#include <iomanip>
#include <ioguard.hpp>
#include <delimiterio.hpp>

namespace muhamadiarov
{
  struct DataStruct
  {
    double key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;

    DataStruct();
  };

  struct DoubleIO
  {
    double& data;
  };

  struct RatIO
  {
    std::pair< long long, unsigned long long >& data;
  };

  struct StringIO
  {
    std::string& data;
  };

  enum class Key
  {
    KEY1,
    KEY2,
    KEY3
  };

  struct KeyIO
  {
    Key& key;
  };

  std::istream& operator>>(std::istream& in, DoubleIO&& d);
  std::istream& operator>>(std::istream& in, RatIO&& r);
  std::istream& operator>>(std::istream& in, StringIO&& s);
  std::istream& operator>>(std::istream& in, KeyIO&& k);

  std::istream& operator>>(std::istream& in, DataStruct& data);
  std::ostream& operator<<(std::ostream& out, const DataStruct& data);

  bool operator<(const DataStruct& lhs, const DataStruct& rhs);

  void check(std::istream& in, char exc);
}
#endif
