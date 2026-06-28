#ifndef UTILS_HPP
#define UTILS_HPP
#include <complex>
#include <string>

namespace saldaev
{
  struct StringIO
  {
    std::string &ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  struct ULL_bin
  {
    unsigned long long &ref;
  };

  struct CMP
  {
    std::complex< double > &ref;
  };

  struct Data
  {
    unsigned long long key1;
    std::complex< double > key2;
    std::string key3;
  };

  std::istream &operator>>(std::istream &in, StringIO &&dest);
  std::istream &operator>>(std::istream &in, LabelIO &&dest);
  std::istream &operator>>(std::istream &in, ULL_bin &&dest);
  std::istream &operator>>(std::istream &in, CMP &&dest);
  std::istream &operator>>(std::istream &in, Data &dest);
  std::ostream &operator<<(std::ostream &out, const Data &dest);

  bool operator<(const Data &lhs, const Data &rhs);
}

#endif
