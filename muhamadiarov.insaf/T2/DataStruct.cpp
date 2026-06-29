#include "DataStruct.hpp"

namespace muh = muhamadiarov;

muh::DataStruct::DataStruct():
  key1(0),
  key2({0, 0}),
  key3()
{}

std::istream& muh::operator>>(std::istream& in, DoubleIO&& d)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> d.data;
  char c = 0;
  in >> c;
  if (c != 'D' && c != 'd')
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& muh::operator>>(std::istream& in, DelimiterIO&& c)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char b = 0;
  in >> std::ws;
  in >> b;
  if (b != c.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& muh::operator>>(std::istream& in, RatIO&& r)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'('} >> DelimiterIO{':'} >> DelimiterIO{'N'};
  if (!in)
  {
    return in;
  }
  in >> r.data.first;
  in >> DelimiterIO{':'} >> DelimiterIO{'D'};
  if (!in)
  {
    return in;
  }
  in >> r.data.second;
  return in >> DelimiterIO{':'} >> DelimiterIO{')'};
}

std::istream& muh::operator>>(std::istream& in, StringIO&& s)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::getline(in >> DelimiterIO{'"'}, s.data, '"');
  return in;
}

std::istream& muh::operator>>(std::istream& in, KeyIO&& k)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string str;
  in >> str;
  if (!in)
  {
    return in;
  }
  if (str == "key1")
  {
    k.key = Key::KEY1;
  }
  else if (str == "key2")
  {
    k.key = Key::KEY2;
  }
  else if (str == "key3")
  {
    k.key = Key::KEY3;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& muh::operator>>(std::istream& in, DataStruct& data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'('};
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;
  for (size_t i = 0; i < 3; ++i)
  {
    in >> DelimiterIO{':'};
    if (!in)
    {
      return in;
    }
    Key key;
    in >> KeyIO{key};
    if (!in)
    {
      return in;
    }
    switch (key)
    {
    case Key::KEY1:
      if (hasKey1)
      {
        in.setstate(std::ios::failbit);
      }
      in >> DoubleIO{data.key1};
      hasKey1 = true;
      break;
    case Key::KEY2:
      if (hasKey2)
      {
        in.setstate(std::ios::failbit);
      }
      in >> RatIO{data.key2};
      hasKey2 = true;
      break;
    case Key::KEY3:
      if (hasKey3)
      {
        in.setstate(std::ios::failbit);
      }
      in >> StringIO{data.key3};
      hasKey3 = true;
      break;
    }
  }
  return in >> DelimiterIO{':'} >> DelimiterIO{')'};
}

std::ostream& muh::operator<<(std::ostream& out, const DataStruct& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  IOguard ioguard(out);
  out << "(:key1 " << std::fixed << std::setprecision(1) << data.key1 << "d";
  out << ":key2 (:N " << data.key2.first << ":D " << data.key2.second << ":)";
  out << ":key3 \"" << data.key3 << "\":)";
  return out;
}

bool muh::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }

  if (lhs.key2.second == 0 || rhs.key2.second == 0)
  {
    return false;
  }

  long double lval = static_cast< long double >(lhs.key2.first) / lhs.key2.second;
  long double rval = static_cast< long double >(rhs.key2.first) / rhs.key2.second;
  if (lval != rval)
  {
    return lval < rval;
  }
  return lhs.key3.size() < rhs.key3.size();
}
