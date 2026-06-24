#include "io_structs.hpp"
#include <cstdlib>
#include <delimiter.hpp>
#include <exception>
#include <ioguard.hpp>
#include <iomanip>
#include <iostream>

std::istream &karpovich::operator>>(std::istream &in, LabelIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  for (size_t i = 0; i < dest.exp.size(); ++i) {
    char c = '\0';
    in >> c;
    if (!in || c != dest.exp[i]) {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  return in;
}

std::istream &karpovich::operator>>(std::istream &in, BinDigitIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = '\0';
  in >> c;
  if (c != '0' && c != '1') {
    in.setstate(std::ios::failbit);
  } else {
    dest.ref = c;
  }
  return in;
}

std::istream &karpovich::operator>>(std::istream &in, BinIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> DelimIO{'0'};
  in >> DelimIO{'b', 'B'};
  if (!in) {
    return in;
  }
  std::string numBin;
  char c = '\0';
  while (in.peek() == '0' || in.peek() == '1') {
    in >> BinDigitIO{c};
    if (!in) {
      return in;
    }
    numBin.push_back(c);
  }
  if (numBin.empty()) {
    in.setstate(std::ios::failbit);
    return in;
  }
  try {
    dest.ref = std::stoull(numBin);
  } catch (const std::exception &) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &karpovich::operator>>(std::istream &in, OctIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  IOguard guard(in);
  in >> DelimIO{'0'} >> std::oct >> dest.ref;
  return in;
}

std::istream &karpovich::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> std::quoted(dest.ref);
}

std::istream &karpovich::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  DataStruct temp;
  bool got1 = false;
  bool got2 = false;
  bool got3 = false;
  in >> DelimIO{'('};
  if (!in) {
    return in;
  }
  while ((!got1 || !got2 || !got3) && in) {
    std::string label;
    in >> label;
    if (label == ":key1" && !got1) {
      in >> BinIO{temp.key1};
      if (in) {
        got1 = true;
      }
    } else if (label == ":key2" && !got2) {
      in >> OctIO{temp.key2};
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

std::ostream &karpovich::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOguard guard(out);
  out << "(:" << "key1 " << "0b" << src.key1 << ":";
  out << "key2 " << '0' << std::oct << src.key2 << ":";
  out << "key3 " << std::quoted(src.key3) << ":" << ")";
  return out;
}

bool karpovich::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2) {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.length() < rhs.key3.length();
}
