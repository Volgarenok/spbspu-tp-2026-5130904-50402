#ifndef IO_GUARD_HPP
#define IO_GUARD_HPP
#include <ios>

namespace novikov
{
  struct IOGuard
  {
  public:
    explicit IOGuard(std::basic_ios< char >&);
    ~IOGuard();

    IOGuard(const IOGuard&) = delete;
    IOGuard& operator=(const IOGuard&) = delete;

    IOGuard(IOGuard&&) = delete;
    IOGuard& operator=(IOGuard&&) = delete;

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}

#endif
