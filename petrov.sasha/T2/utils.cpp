#include "utils.hpp"
#include "ioguard.hpp"

#include <iomanip>
#include <limits>

namespace petrov
{
  std::istream &operator>>(std::istream &in, DelimIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '\0';
    in >> c;
    if (!in || c != dest.exp) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, OctIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string token;
    in >> token;
    if (!in || token.empty()) {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (token[0] != '0') {
      in.setstate(std::ios::failbit);
      return in;
    }
    unsigned long long value = 0;
    for (char c : token) {
      if (c < '0' || c > '7') {
        in.setstate(std::ios::failbit);
        return in;
      }
      unsigned digit = static_cast<unsigned>(c - '0');
      if (value >
        (std::numeric_limits< unsigned long long >::max() - digit) / 8) {
        in.setstate(std::ios::failbit);
        return in;
      }
      value = value * 8 + digit;
    }
    dest.ref = value;
    return in;
  }

  std::istream &operator>>(std::istream &in, HexIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string token;
    in >> token;
    if (!in || token.size() < 3) {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (!(token[0] == '0' &&
          (token[1] == 'x' || token[1] == 'X'))) {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::string digits = token.substr(2);
    if (digits.empty()) {
      in.setstate(std::ios::failbit);
      return in;
    }
    unsigned long long value = 0;
    for (char c : digits) {
      unsigned digit = 0;
      if (c >= '0' && c <= '9') {
        digit = static_cast<unsigned>(c - '0');
      } else if (c >= 'a' && c <= 'f') {
        digit = static_cast<unsigned>(c - 'a' + 10);
      } else if (c >= 'A' && c <= 'F') {
        digit = static_cast<unsigned>(c - 'A' + 10);
      } else {
        in.setstate(std::ios::failbit);
        return in;
      }
      if (value >
        (std::numeric_limits< unsigned long long >::max() - digit) / 16) {
        in.setstate(std::ios::failbit);
        return in;
      }
      value = value * 16 + digit;
    }
    dest.ref = value;
    return in;
  }

  std::istream &operator>>(std::istream &in, StringIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return in >> std::quoted(dest.ref);
  }

  std::istream &operator>>(std::istream &in, DataStruct &dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    DataStruct temp{};
    bool got1 = false;
    bool got2 = false;
    bool got3 = false;
    in >> DelimIO{'('};
    while ((!got1 || !got2 || !got3) && in) {
      std::string label;
      in >> label;
      if (label == ":key1" && !got1) {
        in >> OctIO{temp.key1};
        if (in) {
          got1 = true;
        }
      } else if (label == ":key2" && !got2) {
        in >> HexIO{temp.key2};
        if (in) {
          got2 = true;
        }
      } else if (label == ":key3" && !got3) {
        in >> StringIO{temp.key3};
        if (in) {
          got3 = true;
        }
      } else {
        in.setstate(std::ios::failbit);
      }
    }
    in >> DelimIO{':'} >> DelimIO{')'};
    if (in && got1 && got2 && got3) {
      dest = temp;
    } else {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const DataStruct &src)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }
    IOGuard guard(out);
    out << "(:" << "key1 ";
    out << std::oct << src.key1 << ':';
    out << "key2 " << "0x" << std::uppercase << std::hex << src.key2 << ':';
    out << "key3 " << std::quoted(src.key3) << ':' << ')';
    return out;
  }

  bool operator<(const DataStruct &lhs, const DataStruct &rhs)
  {
    if (lhs.key1 != rhs.key1) {
      return lhs.key1 < rhs.key1;
    }
    if (lhs.key2 != rhs.key2) {
      return lhs.key2 < rhs.key2;
    }
    return lhs.key3.length() < rhs.key3.length();
  }
}

