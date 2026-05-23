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
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DblLitIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    in >> dest.ref;
    char suffix = '0';
    in >> suffix;
    if (in && (suffix != 'd' && suffix != 'D')) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, SllLitIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    in >> dest.ref;
    char suffix1 = '0';
    char suffix2 = '0';
    in >> suffix1 >> suffix2;
    if (in) {
      if (!((suffix1 == 'l' && suffix2 == 'l') || (suffix1 == 'L' && suffix2 == 'L'))) {
        in.setstate(std::ios::failbit);
      }
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, KeyIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string keyName = "";
    char c = '0';
    for (int i = 0; i < 4; ++i) {
      in >> c;
      keyName += c;
    }
    if (keyName == "key1") {
      dest.ref = Key::KEY1;
    } else if (keyName == "key2") {
      dest.ref = Key::KEY2;
    } else if (keyName == "key3") {
      dest.ref = Key::KEY3;
    } else {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
  IoGuard::IoGuard(std::basic_ios<char> &s):
   s_(s),
   fill_(s.fill()),
   precision_(s.precision()),
   fmt_(s.flags())
  {}

  IoGuard::~IoGuard()
  {
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }
}
