#ifndef IOGUARD_HPP
#define IOGUARD_HPP
#include <iostream>

namespace muhamadiarov
{
  struct IOguard
  {
    explicit IOguard(std::basic_ios< char >&);
    ~IOguard();

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}
#endif
