#ifndef IOGUARD_HPP
#define IOGUARD_HPP
#include <ios>
namespace saldaev
{
  class IOguard
  {
  public:
    explicit IOguard(std::basic_ios< char > &s);
    ~IOguard();

  private:
    std::basic_ios< char > &s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}

saldaev::IOguard::IOguard(std::basic_ios< char > &s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{}

saldaev::IOguard::~IOguard()
{
  s_.width(width_);
  s_.precision(precision_);
  s_.flags(fmt_);
  s_.fill(fill_);
}

#endif
