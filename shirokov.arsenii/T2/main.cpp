#include <algorithm>
#include <cctype>
#include <ios>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
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
    const std::vector< DataType >& used;
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
  IOguard g(in);
  while (true)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    in >> std::noskipws;

    DataStruct input;
    std::vector< DataType > used;
    const std::vector< std::string > possibleLabels{"key1", "key2", "key3"};

    in >> sep{'('} >> sep{':'};
    in >> label{used, possibleLabels} >> key{used, input};
    in >> sep{':'};
    in >> label{used, possibleLabels} >> key{used, input};
    in >> sep{':'};
    in >> label{used, possibleLabels} >> key{used, input};
    in >> sep{':'} >> sep{')'};

    if (in)
    {
      dest = input;
      return in;
    }

    in.clear();
    auto toIgnore = std::numeric_limits< std::streamsize >::max();
    in.ignore(toIgnore, '\n');
  }
}

std::istream& shirokov::operator>>(std::istream& in, key&& dest)
{
  std::istream::sentry s(in);
  if (!s || dest.used.empty())
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  DataType currentType = dest.used.back();
  if (std::count(dest.used.begin(), dest.used.end(), currentType) > 1 || currentType == Unknown)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  switch (currentType)
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
  c = c || (lhs.key1 == rhs.key1 && lhs.key2 == rhs.key2 && lhs.key3.length() < rhs.key3.length());
  return c;
}

std::istream& shirokov::operator>>(std::istream& in, sep&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char c = 0;
  if (in.get(c))
  {
    if (c != dest.exp)
    {
      in.putback(c);
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, uncase_sep&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char c = 0;
  if (in.get(c))
  {
    if (std::toupper(static_cast< unsigned char >(c)) != std::toupper(static_cast< unsigned char >(dest.exp))
        && (std::tolower(static_cast< unsigned char >(c)) != std::tolower(static_cast< unsigned char >(dest.exp))))
    {
      in.putback(c);
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, label&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  std::string data;
  in >> data;

  char space = 0;
  if (in.get(space))
  {
    if (space != ' ')
    {
      in.putback(space);
      in.setstate(std::ios::failbit);
    }
  }

  if (!in)
  {
    return in;
  }

  DataType inputType = DataType::Unknown;
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
    in.setstate(std::ios::failbit);
  }

  dest.used.push_back(inputType);
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, ull_oct&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> std::oct >> dest.ref;
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, ull_bin&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }

  in >> sep{'0'} >> uncase_sep{'b'};
  if (!in)
  {
    return in;
  }

  std::string bin_str;
  char c = 0;
  while (in.get(c))
  {
    if (c == '0' || c == '1')
    {
      bin_str += c;
    }
    else
    {
      in.putback(c);
      break;
    }
  }

  if (bin_str.empty())
  {
    in.setstate(std::ios_base::failbit);
  }
  else
  {
    try
    {
      dest.ref = std::stoull(bin_str, nullptr, 2);
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
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> sep{'"'};
  if (!in)
  {
    return in;
  }
  return std::getline(in, dest.ref, '"');
}

std::ostream& shirokov::operator<<(std::ostream& out, const DataStruct& src)
{
  std::string bin_str;
  ull temp = src.key2;
  if (temp == 0)
  {
    bin_str = "0";
  }
  while (temp > 0)
  {
    bin_str = (temp % 2 == 0 ? "0" : "1") + bin_str;
    temp /= 2;
  }

  out << "(:key1 0" << std::oct << src.key1 << std::dec;
  out << ":key2 0b" << bin_str;
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}
