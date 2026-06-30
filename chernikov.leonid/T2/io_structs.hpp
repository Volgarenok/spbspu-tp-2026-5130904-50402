#ifndef IO_STRUCTS_HPP
#define IO_STRUCTS_HPP

#include <complex>
#include <iosfwd>
#include <string>
#include <utility>

namespace chernikov {
  struct DelimIO
  {
    char exp;
  };

  struct LabelIO
  {
    std::string exp;
  };

  struct CmpLspIO
  {
    std::complex< double > &ref;
  };

  struct RatLspIO
  {
    std::pair< long long, unsigned long long > &ref;
  };

  struct StringIO
  {
    std::string &ref;
  };

  std::istream &operator>>(std::istream &in, DelimIO &&dest);
  std::istream &operator>>(std::istream &in, LabelIO &&dest);
  std::istream &operator>>(std::istream &in, CmpLspIO &&dest);
  std::istream &operator>>(std::istream &in, RatLspIO &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
}
#endif
