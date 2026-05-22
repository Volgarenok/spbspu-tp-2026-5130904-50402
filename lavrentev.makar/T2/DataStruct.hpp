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
    long long data;
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
  //std::ostream &operator<<(std::istream &os, Delimiter_t del); хз нужен ли

  std::istream &operator>>(std::istream &is, DataStruct &obj); //FIXME
  std::ostream &operator<<(std::ostream &os, DataStruct obj); //TODO

  std::istream &operator>>(std::istream &is, SllLit &key1);
  std::ostream &operator<<(std::ostream &os, SllLit key1);

  std::istream &operator>>(std::istream &is, UllOct &key2);
  std::ostream &operator<<(std::ostream &os, UllOct key2);

  bool operator<(DataStruct lobj, DataStruct robj); //FIXME
  bool operator<(SllLit lobj, SllLit robj);
  bool operator<(UllOct lobj, UllOct robj);

  bool operator==(SllLit lobj, SllLit robj);
  bool operator==(UllOct lobj, UllOct robj);

  char check(std::istream &is, char expected);
}

#endif
