#include "DataStruct.hpp"
#include <ios>
#include <cstring>
#include <iostream>

lavrentev::IOGuard::IOGuard(std::basic_ios< char > &s):
  s_(s),
  precision_(s.precision()),
  width_(s.width()),
  flags_(s.flags()),
  fill_(s.fill())
{}

lavrentev::IOGuard::~IOGuard()
{
  s_.precision(precision_);
  s_.width(width_);
  s_.flags(flags_);
  s_.fill(fill_);
}

char lavrentev::check(std::istream &is, char expected)
{
  char c = 0;
  is >> c;
  if (c != expected)
  {
    is.setstate(std::ios_base::failbit);
  }
  return c;
}

std::istream &lavrentev::operator>>(std::istream &is, Delimiter_t &del)
{
  del.last = lavrentev::check(is, del.expected);
  return is;
}

std::istream &lavrentev::operator>>(std::istream &is, DataStruct &obj)
{
  std::istream::sentry s(is);
  if (!s)
  {
    return is;
  }
  using d_t = Delimiter_t;
  SllLit x{0};
  UllOct y{0};
  std::string strKey;
  std::string strValue;
  char last = 0;
  d_t dlmInBracket{'(', last};
  d_t dlmColon{':', last};
  d_t dlmOutBracket{')', last};
  is >> dlmInBracket >> dlmColon >> x >> y >> strKey;
  std::getline(is, strValue, ':');
  is >> dlmOutBracket;
  if (is)
  {
    obj = DataStruct{x, y, strValue};
  }
  return is;
}

std::ostream &lavrentev::operator<<(std::ostream &os, Delimiter_t del)
{
  os << del.expected;
  return os;
}

std::ostream &lavrentev::operator<<(std::ostream &os, DataStruct obj)
{
  using d_t = Delimiter_t;
  char last = 0;
  d_t dlmInBracket{'(', last};
  d_t dlmColon{':', last};
  d_t dlmOutBracket{')', last};
  os << dlmInBracket << dlmColon << obj.key1 << obj.key2 << "key3" << obj.key3 << dlmColon << dlmOutBracket;
  return os;
}

std::istream &lavrentev::operator>>(std::istream &is, SllLit &key1)
{
  std::istream::sentry s(is);
  if (!s)
  {
    return is;
  }
  std::string key;
  is >> key;
  if (key != "key1")
  {
    is.setstate(std::ios_base::failbit);
    return is;
  }
  long long value;
  is >> value;

  char postfix[3] = {0};
  if (is.read(postfix, 3))
  {
    if (postfix[2] == ':')
    {
      if (postfix[0] == 'L' && postfix[1] == 'L')
      {
        key1 = SllLit{value, "LL:"};
      }
      else if (postfix[0] == 'l' || postfix[1] == 'l')
      {
        key1 = SllLit{value, "ll:"};
      }
    }
    else
    {
      is.setstate(std::ios::failbit);
    }
  }
  return is;
}

std::ostream &lavrentev::operator<<(std::ostream &os, SllLit key1)
{
  os << "key1 " << key1.data << key1.pf;
  return os;
}

std::istream &lavrentev::operator>>(std::istream &is, UllOct &key2)
{
  std::istream::sentry s(is);
  if (!s)
  {
    return is;
  }
  std::string key;
  is >> key;
  if (key != "key2")
  {
    is.setstate(std::ios_base::failbit);
    return is;
  }
  unsigned long long value;
  is >> std::oct >> value >> std::dec;

  char postfix = 0;
  is >> postfix;
  if (postfix == ':')
  {
    key2 = UllOct{value};
  }
  else
  {
    is.setstate(std::ios_base::failbit);
  }
  return is;
}

std::ostream &lavrentev::operator<<(std::ostream &os, UllOct key2)
{
  os << "key2 0" << std::oct << key2.data << std::dec << ":";
  return os;
}

bool lavrentev::operator<(DataStruct lobj, DataStruct robj)
{
  bool cond = lobj.key1 < robj.key1;
  bool cond2 = lobj.key1 == robj.key1 && lobj.key2 < robj.key2;
  bool cond3 = lobj.key1 == robj.key1 && lobj.key2 == robj.key2 && lobj.key3 < robj.key3;
  return cond || cond2 || cond3;
}

bool lavrentev::operator<(SllLit lobj, SllLit robj)
{
  return lobj.data < robj.data ? true : false;
}

bool lavrentev::operator<(UllOct lobj, UllOct robj)
{
  return lobj.data < robj.data ? true : false;
}

bool lavrentev::operator==(SllLit lobj, SllLit robj)
{
  return lobj.data == robj.data;
}

bool lavrentev::operator==(UllOct lobj, UllOct robj)
{
  return lobj.data == robj.data;
}
