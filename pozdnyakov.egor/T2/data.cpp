#include <cmath>
#include <iomanip>
#include "data.hpp"

namespace pozdnyakov
{

  iofmtguard::iofmtguard(std::basic_ios< char > &s):
    s_(s),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  iofmtguard::~iofmtguard()
  {
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }

  std::istream &operator>>(std::istream &in, LabelIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
      return in;
    for (const char *p = dest.exp; *p != '\0'; ++p) {
      if (in.get() != *p) {
        in.setstate(std::ios::failbit);
        break;
      }
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, KeyIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
      return in;
    dest.ref.clear();
    while (in && std::isalnum(in.peek())) {
      dest.ref += static_cast< char >(in.get());
    }
    if (dest.ref.empty()) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, OctIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
      return in;
    return in >> std::oct >> dest.ref;
  }

  std::istream &operator>>(std::istream &in, RatLspIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
      return in;

    long long n = 0;
    unsigned long long d = 0;

    in >> LabelIO{"(:N"} >> n >> LabelIO{":D"} >> d >> LabelIO{":)"};

    if (in) {
      dest.ref.first = n;
      dest.ref.second = d;
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, StringIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
      return in;
    return in >> std::quoted(dest.ref);
  }

  std::istream &operator>>(std::istream &in, DataStruct &dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
      return in;

    DataStruct input;
    bool has_key1 = false, has_key2 = false, has_key3 = false;

    in >> LabelIO{"(:"};

    for (int i = 0; i < 3; ++i) {
      std::string key;
      in >> KeyIO{key};

      if (key == "key1") {
        in >> OctIO{input.key1};
        has_key1 = true;
      } else if (key == "key2") {
        in >> RatLspIO{input.key2};
        has_key2 = true;
      } else if (key == "key3") {
        in >> StringIO{input.key3};
        has_key3 = true;
      } else {
        in.setstate(std::ios::failbit);
      }

      if (i < 2) {
        in >> LabelIO{":"};
      }
    }

    in >> LabelIO{":)"};

    if (in && has_key1 && has_key2 && has_key3) {
      dest = input;
    } else {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::ostream &operator<<(std::ostream &out, const DataStruct &src)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
      return out;

    iofmtguard fmtguard(out);

    out << "(:key1 " << std::showbase << std::oct << src.key1 << std::dec << ":key2 (:N " << src.key2.first << ":D "
        << src.key2.second << ":)"
        << ":key3 " << std::quoted(src.key3) << ":)";

    return out;
  }

  bool operator<(const DataStruct &lhs, const DataStruct &rhs)
  {
    if (lhs.key1 != rhs.key1) {
      return lhs.key1 < rhs.key1;
    }

    double r1 = static_cast< double >(lhs.key2.first) / lhs.key2.second;
    double r2 = static_cast< double >(rhs.key2.first) / rhs.key2.second;
    if (std::abs(r1 - r2) > 1e-9) {
      return r1 < r2;
    }

    return lhs.key3.length() < rhs.key3.length();
  }

}
