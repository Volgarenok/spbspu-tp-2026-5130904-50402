#ifndef IOGUARD_HPP
#define IOGUARD_HPP
#include <ios>

namespace lavrentev
{
  struct IOGuard
  {
    public:
      explicit IOGuard(std::basic_ios< char > &s);
      ~IOGuard();

      IOGuard(const IOGuard&) = delete;
      IOGuard& operator=(const IOGuard&) = delete;

      IOGuard(IOGuard&&) = delete;
      IOGuard& operator=(IOGuard&&) = delete;
    private:
      std::basic_ios< char > &s_;
      std::streamsize precision_;
      std::streamsize width_;
      std::basic_ios< char >::fmtflags flags_;
      char fill_;
  };
}

#endif
