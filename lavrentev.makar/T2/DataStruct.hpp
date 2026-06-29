#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <istream>
#include <string>

namespace lavrentev
{

  struct SllLit
  {
    long long data;
    std::string pf;
  };

  struct UllOct
  {
    unsigned long long data;
  };

  struct DataStruct
  {
    SllLit key1;
    UllOct key2;
    std::string key3;
  };

  struct Delimiter_t
  {
    char expected;
    char &last;
  };

  std::istream &operator>>(std::istream &is, Delimiter_t &del);
  std::ostream &operator<<(std::ostream &os, Delimiter_t del);

  std::istream &operator>>(std::istream &is, DataStruct &obj);
  std::ostream &operator<<(std::ostream &os, DataStruct obj);

  std::istream &operator>>(std::istream &is, SllLit &key1);
  std::ostream &operator<<(std::ostream &os, SllLit key1);

  std::istream &operator>>(std::istream &is, UllOct &key2);
  std::ostream &operator<<(std::ostream &os, UllOct key2);

  bool operator<(const DataStruct& lobj, const DataStruct& robj);
  bool operator<(const SllLit& lobj, const SllLit& robj);
  bool operator<(const UllOct& lobj, const UllOct& robj);

  bool operator==(const SllLit& lobj, const SllLit& robj);
  bool operator==(const UllOct& lobj, const UllOct& robj);

  void process(std::istream &is, DataStruct &d);
}

#endif
