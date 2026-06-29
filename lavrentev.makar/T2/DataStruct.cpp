#include "DataStruct.hpp"
#include "IOGuard.hpp"
#include <ios>
#include <cstring>
#include <iostream>

std::istream &lavrentev::operator>>(std::istream &is, Delimiter_t &del)
{
  std::istream::sentry s(is);
  if (!s)
  {
    return is;
  }
  char c = 0;
  is >> c;
  if (c != del.expected)
  {
    is.setstate(std::ios_base::failbit);
  }
  del.last = c;
  return is;
}

std::istream &lavrentev::operator>>(std::istream &is, DataStruct &obj)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  using d_t = Delimiter_t;
  char last = 0;

  d_t openBracket{'(', last};
  d_t colon{':', last};
  d_t closeBracket{')', last};

  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;

  is >> openBracket;
  if (!is)
  {
    return is;
  }

  for (int i = 0; i < 3; ++i)
  {
    is >> colon;
    if (!is)
    {
      return is;
    }

    std::string key;
    is >> key;
    if (!is)
    {
      return is;
    }

    if (key == "key1")
    {
      if (hasKey1)
      {
        is.setstate(std::ios::failbit);
        return is;
      }
      hasKey1 = true;
      is >> obj.key1;
    }
    else if (key == "key2")
    {
      if (hasKey2)
      {
        is.setstate(std::ios::failbit);
        return is;
      }
      hasKey2 = true;
      is >> obj.key2;
    }
    else if (key == "key3")
    {
      if (hasKey3)
      {
        is.setstate(std::ios::failbit);
        return is;
      }
      hasKey3 = true;
      is >> obj.key3;
    }
    else
    {
      is.setstate(std::ios::failbit);
      return is;
    }
  }

  if (!(hasKey1 && hasKey2 && hasKey3))
  {
    is.setstate(std::ios::failbit);
    return is;
  }

  is >> colon;
  is >> closeBracket;
  return is;
}

std::ostream &lavrentev::operator<<(std::ostream &os, const Delimiter_t &del)
{
  std::ostream::sentry sentry(os);
  if (!sentry)
  {
    return os;
  }

  os << del.expected;
  return os;
}

std::ostream &lavrentev::operator<<(std::ostream &os, const DataStruct &obj)
{
  std::ostream::sentry sentry(os);
  if (!sentry)
  {
    return os;
  }

  using d_t = Delimiter_t;
  char last = 0;

  d_t openBracket{'(', last};
  d_t colon{':', last};
  d_t closeBracket{')', last};

  os << openBracket << colon << obj.key1 << colon << obj.key2
     << colon << obj.key3 << colon << closeBracket;
  return os;
}

std::istream &lavrentev::operator>>(std::istream &is, SllLit &key1)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  long long value = 0;
  is >> value;
  if (!is)
  {
    return is;
  }

  char l1 = 0;
  char l2 = 0;
  is >> l1 >> l2;

  if (!((l1 == 'l' || l1 == 'L') &&
    (l2 == 'l' || l2 == 'L')))
  {
    is.setstate(std::ios::failbit);
    return is;
  }

  key1 = SllLit{value, "ll"};
  return is;
}

std::ostream &lavrentev::operator<<(std::ostream &os, const SllLit &key1)
{
  std::ostream::sentry sentry(os);
  if (!sentry)
    return os;

  os << "key1 " << key1.data << key1.pf;
  return os;
}

std::istream &lavrentev::operator>>(std::istream &is, UllOct &key2)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  IOGuard guard(is);

  unsigned long long value = 0;
  is >> std::oct >> value;
  if (!is)
  {
    return is;
  }

  key2 = UllOct{value};
  return is;
}

std::ostream &lavrentev::operator<<(std::ostream &os, const UllOct &key2)
{
  std::ostream::sentry sentry(os);
  if (!sentry)
    return os;

  IOGuard guard(os);
  os << "key2 0" << std::oct << key2.data;
  return os;
}

bool lavrentev::operator<(const DataStruct& lobj, const DataStruct& robj)
{
  bool cond = lobj.key1 < robj.key1;
  bool cond2 = lobj.key1 == robj.key1 && lobj.key2 < robj.key2;
  bool cond3 = lobj.key1 == robj.key1 && lobj.key2 == robj.key2 && lobj.key3 < robj.key3;
  return cond || cond2 || cond3;
}

bool lavrentev::operator<(const SllLit& lobj, const SllLit& robj)
{
  return lobj.data < robj.data ? true : false;
}

bool lavrentev::operator<(const UllOct& lobj, const UllOct& robj)
{
  return lobj.data < robj.data ? true : false;
}

bool lavrentev::operator==(const SllLit& lobj, const SllLit& robj)
{
  return lobj.data == robj.data;
}

bool lavrentev::operator==(const UllOct& lobj, const UllOct& robj)
{
  return lobj.data == robj.data;
}

std::istream &lavrentev::operator>>(std::istream &is, StringLit &key3)
{
  char quote = 0;
  is >> quote;

  if (!is || quote != '"')
  {
    is.setstate(std::ios::failbit);
    return is;
  }

  std::getline(is, key3.data, '"');

  return is;
}

std::ostream &lavrentev::operator<<(std::ostream &os, const StringLit &key3)
{
  os << "key3 \"" << key3.data << '"';
  return os;
}

bool lavrentev::operator<(const StringLit& lobj, const StringLit& robj)
{
  return lobj.data.length() < robj.data.length();
}
