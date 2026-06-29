#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <string>
#include <utility>
#include <iostream>
#include <common.hpp>

namespace afanasev
{
  class DataStruct
  {
  public:
    unsigned long long key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };

  struct StringMatch
  {
    std::string exp;
  };

  struct ULLIn
  {
    unsigned long long & num;
  };

  struct RacionalNum
  {
    std::pair< long long, unsigned long long > & num;
  };

  struct Kavichki
  {
    std::string & str;
  };

  std::istream & operator>>(std::istream & in, DataStruct & str);
  std::ostream & operator<<(std::ostream & out, const DataStruct & str);
  bool operator<(const DataStruct & lhs, const DataStruct & rhs);

  std::istream & operator>>(std::istream & in, DelimIO && delim);
  std::istream & operator>>(std::istream & in, StringMatch && str);
  std::istream & operator>>(std::istream & in, ULLIn && str);
  std::istream & operator>>(std::istream & in, RacionalNum && str);
  std::istream & operator>>(std::istream & in, Kavichki && str);
}

#endif
