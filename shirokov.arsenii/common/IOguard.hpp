#ifndef IOGUARD_HPP
#define IOGUARD_HPP
#include <ios>

namespace shirokov
{
  class IOguard
  {
  public:
    explicit IOguard(std::basic_ios< char >&);
    ~IOguard();
    IOguard(IOguard&&) = delete;
    IOguard(const IOguard&) = delete;

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}

#endif
