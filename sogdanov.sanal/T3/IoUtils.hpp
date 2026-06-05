#ifndef IO_UTILS_HPP
#define IO_UTILS_HPP

#include <istream>
#include <ios>

namespace sogdanov {

  struct DelimiterIO {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);

  class IoGuard {
  public:
    explicit IoGuard(std::basic_ios< char >& s);
    ~IoGuard();

  private:
    std::basic_ios< char >& s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

}

#endif
