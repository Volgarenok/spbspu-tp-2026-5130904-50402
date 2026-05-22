#include "DataStruct.hpp"
#include <ios>

lavrentev::IOGuard::IOGuard(std::basic_ios< char > &s):
  s_(s),
  precizion_(s.precision()),
  width_(s.width()),
  flags_(s.flags()),
  fill_(s.fill())
{}

lavrentev::IOGuard::~IOGuard()
{
  s_.precision(precizion_);
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
  std::string str;
  char last = 0;
  d_t dlmInBracket{'(', last};
  d_t dlmColon{':', last};
  d_t dlmOutBracket{'(', last};
  is >> dlmInBracket >> dlmColon >> x >> y >> str >> dlmOutBracket;
  if (is)
  {
    obj = DataStruct{x, y, str};
  }
  return is;
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
    if (strcmp(postfix, "LL:") == 0 || strcmp(postfix, "ll:") == 0)
    {
      key1 = SllLit{value};
    }
    else
    {
      is.setstate(std::ios::failbit);
    }
  }
  return is;
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
  is >> value;

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

bool lavrentev::operator<(DataStruct lobj, DataStruct robj)
{
  if (lobj.key1 < robj.key1)
  {
    return true;
  }
  if (lobj.key1 == robj.key1)
  {
    if (lobj.key2 < robj.key2)
    {
      return true;
    }
    if (lobj.key2 == robj.key2)
    {
      if (lobj.key3 < robj.key3)
      {
        return true;
      }
    }
  }
  return false;
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
