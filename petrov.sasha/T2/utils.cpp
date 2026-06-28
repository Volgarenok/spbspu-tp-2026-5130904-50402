#include "utils.hpp"

#include <iomanip>
#include <istream>
#include <ostream>

#include "ioguard.hpp"

std::istream &petrov::operator>>(std::istream &in, DelimIO &&dest)
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

std::istream &petrov::operator>>(std::istream &in, OctIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  IOGuard guard(in);
  in >> DelimIO{'0'} >> std::oct >> dest.ref;
  return in;
}

std::istream &petrov::operator>>(std::istream &in, HexIO &&dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  char zero = '\0';
  char x = '\0';
  in >> zero >> x;
  if (!in || zero != '0' || (x != 'x' && x != 'X')) {
    in.setstate(std::ios::failbit);
    return in;
  }
  IOGuard guard(in);
  in >> std::hex >> dest.ref;
  return in;
}

std::istream &petrov::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> std::quoted(dest.ref);
}

std::istream &petrov::operator>>(std::istream &in, DataStruct &dest)
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
  if (!in) {
    return in;
  }
  while (in && (!got1 || !got2 || !got3)) {
    in >> DelimIO{':'};
    if (!in) {
      break;
    }
    std::string key;
    in >> key;
    if (!in) {
      break;
    }
    if (key == "key1" && !got1) {
      in >> OctIO{temp.key1};
      if (in) {
        got1 = true;
      }
    }
    else if (key == "key2" && !got2) {
      in >> HexIO{temp.key2};
      if (in) {
        got2 = true;
      }
    }
    else if (key == "key3" && !got3) {
      in >> StringIO{temp.key3};
      if (in) {
        got3 = true;
      }
    }
    else {
      in.setstate(std::ios::failbit);
      break;
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

std::ostream &petrov::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOGuard guard(out);
  out << "(:" << "key1 ";
  out << '0' << std::oct << src.key1 << ':';
  out << "key2 " << "0x" << std::uppercase << std::hex << src.key2 << ':';
  out << "key3 " << std::quoted(src.key3) << ':' << ')';
  return out;
}

bool petrov::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2) {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.length() < rhs.key3.length();
}

