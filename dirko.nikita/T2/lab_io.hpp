#ifndef LAB_IO_HPP
#define LAB_IO_HPP
#include <complex>
#include <iosfwd>
#include <string>
namespace dirko
{
  struct DataStruct
  {
    char key1;
    std::complex< double > key2;
    std::string key3;
  };
  struct CharIO
  {
    char &ref;
  };
  struct CompIO
  {
    std::complex< double > &ref;
  };
  struct StringIO
  {
    std::string &ref;
  };
  struct LabelIO
  {
    std::string exp;
  };
  std::istream &operator>>(std::istream &in, CharIO &&dest);
  std::istream &operator>>(std::istream &in, CompIO &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
  std::istream &operator>>(std::istream &in, LabelIO &&dest);

  std::istream &operator>>(std::istream &in, DataStruct &dest);
  std::ostream &operator<<(std::ostream &out, const DataStruct &src);

  bool operator<(const DataStruct &lhs, const DataStruct &rhs);
}
#endif
