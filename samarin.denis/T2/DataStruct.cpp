#include "DataStruct.hpp"

#include <cctype>
#include <cstddef>
#include <iomanip>
#include <ios>
#include <istream>
#include <ostream>
#include <string>
#include "DelimiterIO.hpp"
#include "StreamGuard.hpp"

namespace {
  using samarin::DelimiterIO;
  using samarin::StreamGuard;

  struct LabelIO {
    std::string &ref;
  };

  struct UllLiteralIO {
    unsigned long long &ref;
  };

  struct UllOctalIO {
    unsigned long long &ref;
  };

  struct StringIO {
    std::string &ref;
  };

  std::istream &operator>>(std::istream &in, LabelIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    in >> dest.ref;
    return in;
  }

  std::istream &operator>>(std::istream &in, UllLiteralIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    if (!std::isdigit(static_cast< unsigned char >(in.peek()))) {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> dest.ref;
    const char suffix[] = {'u', 'l', 'l'};
    for (const char expected: suffix) {
      const char received = static_cast< char >(in.get());
      if (!in || (std::tolower(static_cast< unsigned char >(received)) != expected)) {
        in.setstate(std::ios::failbit);
        break;
      }
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, UllOctalIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    if (in.peek() != '0') {
      in.setstate(std::ios::failbit);
      return in;
    }
    StreamGuard guard(in);
    in >> std::oct >> dest.ref;
    return in;
  }

  std::istream &operator>>(std::istream &in, StringIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    in >> std::quoted(dest.ref);
    return in;
  }
}

std::istream &samarin::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  const std::size_t fieldCount = 3;
  DataStruct input{};
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;
  in >> DelimiterIO{'('};
  for (std::size_t i = 0; i < fieldCount; ++i) {
    in >> DelimiterIO{':'};
    std::string label;
    in >> LabelIO{label};
    if (label == "key1") {
      in >> UllLiteralIO{input.key1};
      hasKey1 = true;
    } else if (label == "key2") {
      in >> UllOctalIO{input.key2};
      hasKey2 = true;
    } else if (label == "key3") {
      in >> StringIO{input.key3};
      hasKey3 = true;
    } else {
      in.setstate(std::ios::failbit);
    }
  }
  in >> DelimiterIO{':'} >> DelimiterIO{')'};
  if (in && hasKey1 && hasKey2 && hasKey3) {
    dest = input;
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream &samarin::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  StreamGuard guard(out);
  out << "(:key1 " << src.key1 << "ull";
  out << ":key2 0" << std::oct << src.key2;
  out << ":key3 " << std::quoted(src.key3);
  out << ":)";
  return out;
}

bool samarin::compareData(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2) {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.length() < rhs.key3.length();
}
