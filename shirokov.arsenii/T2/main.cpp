#include <algorithm>
#include <cctype>
#include <ios>
#include <iostream>
#include <istream>
#include <iterator>
#include <string>
#include <vector>

namespace shirokov
{
  using ull = unsigned long long;

  struct DataStruct
  {
    ull key1;
    ull key2;
    std::string key3;
  };

  enum DataType
  {
    UllOct,
    UllBin,
    String,
    Unknown
  };

  struct UllOctIO
  {
    ull& ref;
  };

  struct UllBinIO
  {
    ull& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct KeyIO
  {
    const DataType& type;
    DataStruct& input;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct DelimiterUnCaseIO
  {
    char exp;
  };

  struct LabelIO
  {
    std::vector< DataType >& used;
    const std::vector< std::string >& possibleLabels;
  };

  class IOguard
  {
  public:
    explicit IOguard(std::basic_ios< char >&);
    ~IOguard();

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };

  using sep = DelimiterIO;
  using uncase_sep = DelimiterUnCaseIO;
  using label = LabelIO;
  using ull_oct = UllOctIO;
  using ull_bin = UllBinIO;
  using str = StringIO;
  using key = KeyIO;

  std::istream& operator>>(std::istream&, sep&&);
  std::istream& operator>>(std::istream&, uncase_sep&&);
  std::istream& operator>>(std::istream&, ull_oct&&);
  std::istream& operator>>(std::istream&, ull_bin&&);
  std::istream& operator>>(std::istream&, label&&);
  std::istream& operator>>(std::istream&, str&&);
  std::istream& operator>>(std::istream&, key&&);
  std::istream& operator>>(std::istream&, DataStruct&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);
  bool compare(const DataStruct&, const DataStruct&);
}

int main()
{
  std::vector< shirokov::DataStruct > data;

  {
    using iit_t = std::istream_iterator< shirokov::DataStruct >;
    std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));
  }

  {
    std::sort(std::begin(data), std::end(data), shirokov::compare);
    using oit_t = std::ostream_iterator< shirokov::DataStruct >;
    std::copy(std::begin(data), std::end(data), oit_t{std::cout, "\n"});
  }
}

std::istream& shirokov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  IOguard g(in);

  DataStruct input;
  {
    std::vector< DataType > used;
    const std::vector< std::string > possibleLabels{"key1", "key2", "key3"};
    in >> sep{'('} >> sep{':'};
    in >> label{used, possibleLabels} >> key{used.back(), input};
    in >> sep{':'};
    in >> label{used, possibleLabels} >> key{used.back(), input};
    in >> sep{':'};
    in >> label{used, possibleLabels} >> key{used.back(), input};
    in >> sep{':'} >> sep{')'};
  }

  if (in)
  {
    dest = input;
  }
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, key&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  switch (dest.type)
  {
  case UllOct:
    in >> ull_oct{dest.input.key1};
    break;
  case UllBin:
    in >> ull_bin{dest.input.key2};
    break;
  case String:
    in >> str{dest.input.key3};
    break;
  default:
    in.setstate(std::ios::failbit);
    break;
  }
  return in;
}

shirokov::IOguard::IOguard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{}

shirokov::IOguard::~IOguard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

bool shirokov::compare(const DataStruct& lhs, const DataStruct& rhs)
{
  bool c = lhs.key1 < rhs.key1;
  c = c || (lhs.key1 == rhs.key1 && lhs.key2 < rhs.key2);
  c = c || (lhs.key1 == rhs.key1 && lhs.key2 == rhs.key2 && lhs.key3 < rhs.key3);
  return c;
}

std::istream& shirokov::operator>>(std::istream& in, sep&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, label&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string data;
  in >> data;
  DataType inputType;
  if (data == dest.possibleLabels[0])
  {
    inputType = DataType::UllOct;
  }
  else if (data == dest.possibleLabels[1])
  {
    inputType = DataType::UllBin;
  }
  else if (data == dest.possibleLabels[2])
  {
    inputType = DataType::String;
  }
  else
  {
    inputType = DataType::Unknown;
    in.setstate(std::ios::failbit);
  }
  dest.used.push_back(inputType);
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, ull_oct&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> std::oct >> dest.ref;
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, ull_bin&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> sep{'0'} >> uncase_sep{'b'};
  std::string buffer;
  if (in >> buffer)
  {
    try
    {
      dest.ref = std::stoull(buffer, nullptr, 2);
    }
    catch (...)
    {
      in.setstate(std::ios_base::failbit);
    }
  }
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, str&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> sep{'"'}, dest.ref, '"');
}

std::istream& shirokov::operator>>(std::istream& in, uncase_sep&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (std::toupper(c) != dest.exp) && (std::tolower(c) != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
