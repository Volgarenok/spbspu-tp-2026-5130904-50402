#ifndef IO_STRUCTS_HPP
#define IO_STRUCTS_HPP

#include <iosfwd>
#include <string>

namespace chernikov {

  struct StringIO
  {
    std::string &ref_;
    explicit StringIO(std::string &ref);
  };

  struct ValueIO
  {
    double &ref_;
    explicit ValueIO(double &ref);
  };

  std::istream &operator>>(std::istream &in, StringIO &&value);
  std::istream &operator>>(std::istream &in, ValueIO &&value);

}

#endif
