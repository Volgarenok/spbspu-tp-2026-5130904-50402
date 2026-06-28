#ifndef UTILS_HPP
#define UTILS_HPP

#include <iosfwd>
#include <string>

namespace petrov
{
  struct DataStruct
  {
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
  };

  struct OctIO
  {
    unsigned long long &ref;
  };

  struct HexIO
  {
    unsigned long long &ref;
  };

  struct StringIO
  {
    std::string &ref;
  };

  std::istream &operator>>(std::istream &in, OctIO &&dest);
  std::istream &operator>>(std::istream &in, HexIO &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);

  std::istream &operator>>(std::istream &in, DataStruct &dest);
  std::ostream &operator<<(std::ostream &out, const DataStruct &src);

  bool operator<(const DataStruct &lhs, const DataStruct &rhs);
}

#endif
