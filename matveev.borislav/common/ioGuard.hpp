#ifndef IO_GUARD_HPP
#define IO_GUARD_HPP

#include <ios>

namespace matveev
{
  class IOGuard
  {
  public:
    explicit IOGuard(std::basic_ios< char >& stream);
    IOGuard(const IOGuard&) = delete;
    IOGuard(IOGuard&&) = delete;
    IOGuard& operator=(const IOGuard&) = delete;
    IOGuard& operator=(IOGuard&&) = delete;
    ~IOGuard();

  private:
    std::basic_ios< char >& stream_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
    std::streamsize precision_;
  };
}

#endif
