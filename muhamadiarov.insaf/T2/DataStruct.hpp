#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <string>
#include <iostream>
#include <utility>
#include <iomanip>

namespace muhamadiarov
{
  struct DataStruct
  {
    double key1_;
    std::pair< long long, unsigned long long > key2_;
    std::string key3_;
  };

  struct DoubleIO
  {
    double& data_;
  };

  struct RatIO
  {
    std::pair< long long, unsigned long long >& data_;
  };

  struct StringIO
  {
    std::string& data_;
  };

  struct DelimiterIO
  {
    char exp_;
  };

  enum class Key
  {
    KEY1,
    KEY2,
    KEY3
  };

  struct KeyIO
  {
    Key& key_;
  };

  std::istream& operator>>(std::istream& in, DoubleIO&& d);
  std::istream& operator>>(std::istream& in, RatIO&& r);
  std::istream& operator>>(std::istream& in, StringIO&& s);
  std::istream& operator>>(std::istream& in, DelimiterIO&& c);
  std::istream& operator>>(std::istream& in, KeyIO& k);

  std::istream& operator>>(std::istream& in, DataStruct&& data);
  std::ostream& operator<<(std::ostream& out, const DataStruct& data);

  bool operator<(const DataStruct& lhs, const DataStruct& rhs);

  void check(std::istream& in, char exc);
}

namespace muh = muhamadiarov;

void muh::check(std::istream& in, char exc)
{
  char c = 0;
  in >> c;
  if (c != exc)
  {
    in.setstate(std::ios::failbit);
  }
}

std::istream& muh::operator>>(std::istream& in, DoubleIO&& d)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> d.data_;
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
  check(in, c.exp_);
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
  in >> r.data_.first;
  in >> DelimiterIO{':'} >> DelimiterIO{'D'};
  if (!in)
  {
    return in;
  }
  in >> r.data_.second;
  return in >> DelimiterIO{':'} >> DelimiterIO{')'};
}

std::istream& muh::operator>>(std::istream& in, StringIO&& s)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'"'};
  if (!in)
  {
    return in;
  }
  std::getline(in, s.data_, '"');
  return in;
}

std::istream& muh::operator>>(std::istream& in, KeyIO& k)
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
    k.key_ = Key::KEY1;
  }
  else if (str == "key2")
  {
    k.key_ = Key::KEY2;
  }
  else if (str == "key3")
  {
    k.key_ = Key::KEY3;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& muh::operator>>(std::istream& in, DataStruct&& data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'('} >> DelimiterIO{':'};
  for (size_t i = 0; i < 3; ++i)
  {
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
        in >> DoubleIO{data.key1_};
        break;
      case Key::KEY2:
        in >> RatIO{data.key2_};
        break;
      case Key::KEY3:
        in >> StringIO{data.key3_};
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
  out << "(:key1 " << data.key1_ << "d";
  out << ":key2 (:N " << data.key2_.first << ":D " << data.key2_.second << ":)";
  out << ":key3 \"" << data.key3_ << "\":)";
  return out;
}
#endif
