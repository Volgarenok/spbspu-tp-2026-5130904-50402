#ifndef IOGUARD_HPP
#define IOGUARD_HPP

#include <ios>

namespace chernikov {
  struct IOguard
  {
    explicit IOguard(std::basic_ios< char > &stream);
    ~IOguard();

  private:
    std::basic_ios< char > &stream_;
    std::ios_base::fmtflags flags_;
    std::streamsize precision_;
  };
}
#endif
