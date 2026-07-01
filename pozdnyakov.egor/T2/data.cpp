#include "data.hpp"
#include <cmath>
#include <iomanip>
#include "IoFmtGuard.hpp"

std::istream &pozdnyakov::operator>>(std::istream &in, pozdnyakov::LabelIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  for (const char *p = dest.exp; *p != '\0'; ++p) {
    char c = '\0';
    in.get(c);
    if (c != *p) {
      in.setstate(std::ios::failbit);
      break;
    }
  }
  return in;
}

std::istream &pozdnyakov::operator>>(std::istream &in, pozdnyakov::KeyIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  dest.ref.clear();
  char c = '\0';
  while (in.get(c)) {
    if (std::isalnum(c)) {
      dest.ref += c;
    } else {
      in.unget();
      break;
    }
  }

  if (dest.ref.empty()) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &pozdnyakov::operator>>(std::istream &in, pozdnyakov::OctIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  char c = '\0';
  in.get(c);

  if (in && c == '0') {
    in.unget();
    return in >> std::oct >> dest.ref;
  }

  in.setstate(std::ios::failbit);
  return in;
}

std::istream &pozdnyakov::operator>>(std::istream &in, pozdnyakov::RatLspIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  long long numerator = 0;
  unsigned long long denominator = 0;

  in >> pozdnyakov::LabelIO{"(:N"};
  in >> numerator;
  in >> pozdnyakov::LabelIO{":D"};
  in >> denominator;
  in >> pozdnyakov::LabelIO{":)"};

  if (in) {
    dest.ref.first = numerator;
    dest.ref.second = denominator;
  }
  return in;
}

std::istream &pozdnyakov::operator>>(std::istream &in, pozdnyakov::StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  return in >> std::quoted(dest.ref);
}

std::istream &pozdnyakov::operator>>(std::istream &in, pozdnyakov::DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  pozdnyakov::DataStruct inputData{0, {0, 0}, ""};
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;

  in >> pozdnyakov::LabelIO{"(:"};

  for (int i = 0; i < 3; ++i) {
    std::string currentKey;
    in >> pozdnyakov::KeyIO{currentKey};

    if (currentKey == "key1") {
      in >> pozdnyakov::OctIO{inputData.key1};
      hasKey1 = true;
    } else if (currentKey == "key2") {
      in >> pozdnyakov::RatLspIO{inputData.key2};
      hasKey2 = true;
    } else if (currentKey == "key3") {
      in >> pozdnyakov::StringIO{inputData.key3};
      hasKey3 = true;
    } else {
      in.setstate(std::ios::failbit);
    }

    if (i < 2) {
      in >> pozdnyakov::LabelIO{":"};
    }
  }

  in >> pozdnyakov::LabelIO{":)"};

  if (in && hasKey1 && hasKey2 && hasKey3) {
    dest = inputData;
  } else {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream &pozdnyakov::operator<<(std::ostream &out, const pozdnyakov::DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }

  pozdnyakov::IoFmtGuard fmtGuard(out);

  out << "(:key1 0" << std::oct << src.key1 << std::dec;
  out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
  out << ":key3 " << std::quoted(src.key3) << ":)";

  return out;
}

bool pozdnyakov::operator<(const pozdnyakov::DataStruct &lhs, const pozdnyakov::DataStruct &rhs)
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
