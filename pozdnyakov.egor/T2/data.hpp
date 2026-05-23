#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
#include <string>
#include <utility>

namespace pozdnyakov
{

  struct DataStruct
  {
    unsigned long long key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;                        
  };

  std::istream &operator>>(std::istream &in, DataStruct &dest);
  std::ostream &operator<<(std::ostream &out, const DataStruct &src);

  bool operator<(const DataStruct &lhs, const DataStruct &rhs);

  class iofmtguard
  {
  public:
    explicit iofmtguard(std::basic_ios< char > &s);
    ~iofmtguard();

  private:
    std::basic_ios< char > &s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  struct LabelIO
  {
    const char *exp;
  };
  std::istream &operator>>(std::istream &in, LabelIO &&dest);

  struct KeyIO
  {
    std::string &ref;
  };
  std::istream &operator>>(std::istream &in, KeyIO &&dest);

  struct OctIO
  {
    unsigned long long &ref;
  };
  std::istream &operator>>(std::istream &in, OctIO &&dest);

  struct RatLspIO
  {
    std::pair< long long, unsigned long long > &ref;
  };
  std::istream &operator>>(std::istream &in, RatLspIO &&dest);

  struct StringIO
  {
    std::string &ref;
  };
  std::istream &operator>>(std::istream &in, StringIO &&dest);

}

#endif
