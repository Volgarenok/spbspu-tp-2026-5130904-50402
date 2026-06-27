#ifndef IOGUARD_HPP
#define IOGUARD_HPP

#include <ios>

namespace sogdanov
{
  class IoGuard
  {
  public:
    explicit IoGuard(std::basic_ios< char >& s);
    ~IoGuard();

    IoGuard(const IoGuard&) = delete;
    IoGuard& operator=(const IoGuard&) = delete;
    IoGuard(IoGuard&&) = delete;
    IoGuard& operator=(IoGuard&&) = delete;

  private:
    std::basic_ios< char >& s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
