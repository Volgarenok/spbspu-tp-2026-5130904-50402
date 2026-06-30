#ifndef IO_UTILS_HPP
#define IO_UTILS_HPP

#include <ios>
#include <istream>

namespace chernikov {
  struct DelimIO
  {
    char exp;
  };

  struct IOguard
  {
    explicit IOguard(std::basic_ios< char > &stream);
    ~IOguard();

  private:
    std::basic_ios< char > &stream_;
    std::ios_base::fmtflags flags_;
    std::streamsize precision_;
  };

  std::istream &operator>>(std::istream &in, DelimIO &&dest);
}
#endif
