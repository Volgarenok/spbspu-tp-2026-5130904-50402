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
  is >> d_t{'(', last} >> d_t{':', last} >> x >> y >> str >> d_t{')', last};
  if (is)
  {
    obj = DataStruct{x, y, str};
  }
  return is;
}
