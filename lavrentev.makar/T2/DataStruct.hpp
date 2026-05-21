#include <istream>
#include <string>

namespace lavrentev
{
  struct SllLit
  {
    long long &data;
  };

  struct UllOct
  {
    long long &data;
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

  std::istream &operator>>(std::istream &is, DataStruct &obj);
  std::ostream &operator<<(std::ostream &os, DataStruct obj);

  bool operator<(DataStruct lobj, DataStruct robj);
}