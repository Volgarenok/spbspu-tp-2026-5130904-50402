#ifndef IOGUARD_HPP
#define IOGUARD_HPP

#include <ios>

namespace petrov
{
  class IOGuard
  {
  public:
    IOGuard(const IOGuard &) = delete;
    IOGuard(IOGuard &&) = delete;
    explicit IOGuard(std::basic_ios< char > &stream);
    ~IOGuard();
    IOGuard &operator=(const IOGuard &) = delete;
    IOGuard &operator=(IOGuard &&) = delete;

  private:
    std::basic_ios< char > &stream_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };
}

#endif
