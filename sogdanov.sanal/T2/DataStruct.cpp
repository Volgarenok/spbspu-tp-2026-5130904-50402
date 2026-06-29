#include "DataStruct.hpp"
#include "../common/IoGuard.hpp"
#include <iomanip>
#include <string>

std::istream& sogdanov::operator>>(std::istream& in, DelimiterIO&& dest)
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

std::istream& sogdanov::operator>>(std::istream& in, DblLitIO&& dest)
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

std::istream& sogdanov::operator>>(std::istream& in, SllLitIO&& dest)
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

std::istream& sogdanov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream& sogdanov::operator>>(std::istream& in, KeyIO&& dest)
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

std::istream& sogdanov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  DataStruct input;
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;

  in >> DelimiterIO{'('} >> DelimiterIO{':'};

  for (int i = 0; i < 3; ++i) {
    Key currentKey;
    in >> KeyIO{currentKey};
    switch (currentKey) {
    case Key::KEY1:
      if (hasKey1) {
        in.setstate(std::ios::failbit);
      } else {
        in >> DblLitIO{input.key1};
        hasKey1 = true;
      }
      break;
    case Key::KEY2:
      if (hasKey2) {
        in.setstate(std::ios::failbit);
      } else {
        in >> SllLitIO{input.key2};
        hasKey2 = true;
      }
      break;
    case Key::KEY3:
      if (hasKey3) {
        in.setstate(std::ios::failbit);
      } else {
        in >> StringIO{input.key3};
        hasKey3 = true;
      }
      break;
    }
    in >> DelimiterIO{':'};
  }

  in >> DelimiterIO{')'};

  if (in && hasKey1 && hasKey2 && hasKey3) {
    dest = input;
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& sogdanov::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IoGuard guard(out);
  out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
  out << ":key2 " << src.key2 << "ll";
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}

bool sogdanov::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2) {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.length() < rhs.key3.length();
}
