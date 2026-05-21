#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <ios>
#include <istream>
#include <string>

namespace lavrentev
{
  struct IOGuard
  {
    public:
      explicit IOGuard(std::basic_ios< char > &s);
      ~IOGuard();
    private:
      std::basic_ios< char > &s_;
      std::streamsize precizion_;
      std::streamsize width_;
      std::basic_ios< char >::fmtflags flags_;
      char fill_;
  };

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

#endif
