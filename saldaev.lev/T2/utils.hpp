#ifndef UTILS_HPP
#define UTILS_HPP
#include <complex>
#include <iomanip>
#include <string>

namespace saldaev
{
  class IOguard
  {
  public:
    explicit IOguard(std::basic_ios< char > &s);
    ~IOguard();

  private:
    std::basic_ios< char > &s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };

  IOguard::IOguard(std::basic_ios< char > &s):
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  IOguard::~IOguard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }

  struct DelimiterIO
  {
    char exp;
  };

  struct StringIO
  {
    std::string &ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
  std::istream &operator>>(std::istream &in, LabelIO &&dest);

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

  std::istream &operator>>(std::istream &in, ULL_bin &&dest);
  std::istream &operator>>(std::istream &in, CMP &&dest);
  std::istream &operator>>(std::istream &in, Data &&dest);
  std::ostream &operator<<(std::ostream &out, const Data &dest);
}

#endif
