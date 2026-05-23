#include "DataStruct.hpp"
#include <iomanip>
#include <string>

namespace sogdanov
{
  struct DelimiterIO
  {
    char exp;
  };

  struct DblLitIO
  {
    double &ref;
  };

  struct SllLitIO
  {
    long long &ref;
  };

  struct StringIO
  {
    std::string &ref;
  };

  enum class Key
  {
    KEY1,
    KEY2,
    KEY3
  };

  struct KeyIO
  {
    Key &ref;
  };

  class IoGuard
  {
  public:
    explicit IoGuard(std::basic_ios<char> &s);
    ~IoGuard();

  private:
    std::basic_ios<char> &s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
  };
}
sogdanov::IoGuard::IoGuard(std::basic_ios<char> &s):
  s_(s),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

sogdanov::IoGuard::~IoGuard()
{
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
