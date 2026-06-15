#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>

namespace khalikov
{

  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct DoubleIO
  {
    double &ref;
  };

  struct UllIO
  {
    unsigned long long &ref;
  };

  struct StringIO
  {
    std::string &ref;
  };

  enum class Key { KEY1, KEY2, KEY3 };

  struct KeyIO
  {
    Key &ref;
  };

  class IoGuard
  {
  public:
    explicit IoGuard(std::basic_ios< char > &s);
    ~IoGuard();

  private:
    std::basic_ios< char > &s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };

  void check(std::istream &in, bool &flag);

  bool operator<(const DataStruct &lhs, const DataStruct &rhs);
  std::istream &operator>>(std::istream &in, DoubleIO &&dest);
  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, UllIO &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
  std::istream &operator>>(std::istream &in, KeyIO &&dest);
  std::ostream &operator<<(std::ostream &out, const DataStruct &src);
  std::istream &operator>>(std::istream &in, DataStruct &dest);
}

int main()
{
  std::vector< khalikov::DataStruct > data;
  {
    using iit_t = std::istream_iterator< khalikov::DataStruct >;
    while (!std::cin.eof()) {
      std::copy(iit_t(std::cin), iit_t(), std::back_inserter(data));
      if (std::cin.fail() && !std::cin.eof()) {
        std::cin.clear();
        auto toIgnore = std::numeric_limits< std::streamsize >::max();
        std::cin.ignore(toIgnore, '\n');
      }
    }
  }
  std::sort(data.begin(), data.end());
  {
    using oit_t = std::ostream_iterator< khalikov::DataStruct >;
    std::copy(data.begin(), data.end(), oit_t(std::cout, "\n"));
  }
}

void khalikov::check(std::istream &in, bool &flag)
{
  if (flag) {
    in.setstate(std::ios::failbit);
  }
  flag = true;
}

bool khalikov::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  bool c = lhs.key1 < rhs.key1;
  c = c || (lhs.key1 == rhs.key1 && lhs.key2 < rhs.key2);
  c = c || (lhs.key1 == rhs.key1 && lhs.key2 == rhs.key2 && lhs.key3.length() < rhs.key3.length());
  return c;
}

std::istream &khalikov::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  DataStruct input;
  bool hk1 = false;
  bool hk2 = false;
  bool hk3 = false;
  {
    using sep = DelimiterIO;
    using label = KeyIO;
    using dbl = DoubleIO;
    using str = StringIO;
    using ull = UllIO;
    in >> sep{'('} >> sep{':'};
    for (size_t i = 0; i < 3; ++i) {
      Key key;
      in >> label{key};
      switch (key) {
      case Key::KEY1:
        check(in, hk1);
        in >> dbl{input.key1};
        break;
      case Key::KEY2:
        check(in, hk2);
        in >> ull{input.key2};
        break;
      case Key::KEY3:
        check(in, hk3);
        in >> str{input.key3};
        break;
      }
      in >> sep{':'};
    }
    in >> sep{')'};
    if (in && hk1 && hk2 && hk3) {
      dest = input;
    } else {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::istream &khalikov::operator>>(std::istream &in, KeyIO &&dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  std::string curr = "";
  char c = '0';
  for (size_t i = 0; i < 4; ++i) {
    in >> c;
    curr += c;
  }
  if (curr == "key1") {
    dest.ref = Key::KEY1;
  } else if (curr == "key2") {
    dest.ref = Key::KEY2;
  } else if (curr == "key3") {
    dest.ref = Key::KEY3;
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &khalikov::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream &khalikov::operator>>(std::istream &in, UllIO &&dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  in >> std::oct >> dest.ref >> std::dec;
  return in;
}

std::istream &khalikov::operator>>(std::istream &in, DoubleIO &&dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  in >> dest.ref;
  char suf = '0';
  in >> suf;
  if (in && (suf != 'd' && suf != 'D')) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &khalikov::operator>>(std::istream &in, DelimiterIO &&dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream &khalikov::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry s(out);
  if (!s) {
    return out;
  }
  IoGuard fmtguard(out);
  out << "(:";
  out << "key1 " << std::fixed << std::setprecision(1) << src.key1 << 'd';
  out << ":key2 " << std::oct << std::showbase << src.key2;
  out << ":key3 \"" << src.key3 << '"';
  out << ":)";
  return out;
}

khalikov::IoGuard::IoGuard(std::basic_ios< char > &s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{}

khalikov::IoGuard::~IoGuard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
