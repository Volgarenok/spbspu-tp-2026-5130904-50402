#ifndef IO_STRUCTS_HPP
#define IO_STRUCTS_HPP

#include <iosfwd>
#include <complex>
#include <utility>
#include <string>

namespace chernikov
{

  struct ComplexIO
  {
    std::complex<double> &ref_;
    explicit ComplexIO(std::complex<double> &ref);
  };

  struct RationalIO
  {
    std::pair<long long, unsigned long long> &ref_;
    explicit RationalIO(std::pair<long long, unsigned long long> &ref);
  };

  struct StringIO
  {
    std::string &ref_;
    explicit StringIO(std::string &ref);
  };

  std::istream &operator>>(std::istream &in, ComplexIO &&value);
  std::istream &operator>>(std::istream &in, RationalIO &&value);
  std::istream &operator>>(std::istream &in, StringIO &&value);

}

#endif
