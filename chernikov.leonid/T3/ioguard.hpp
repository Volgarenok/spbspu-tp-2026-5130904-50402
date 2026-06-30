#ifndef IOGUARD_HPP
#define IOGUARD_HPP
#include <ios>

namespace chernikov {
  class IOguard
  {
  public:
    IOguard(const IOguard &) = delete;
    IOguard(IOguard &&) = delete;
    explicit IOguard(std::basic_ios< char > &s);
    ~IOguard();
    IOguard &operator=(const IOguard &) = delete;
    IOguard &operator=(IOguard &&) = delete;

  private:
    std::basic_ios< char > &s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}
#endif
