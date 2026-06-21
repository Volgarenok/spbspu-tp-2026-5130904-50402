#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace novikov
{
  struct DataStruct
  {
    double key1;
    std::complex<double> key2;
    std::string key3;
  };

  enum DataType
  {
    DblLit,
    CmpLsp,
    String,
    Unknown
  };

  struct DblLitIO
  {
    double& ref;
  };

  struct CmpLspIO
  {
    std::complex<double>& ref;
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
  using dbl_lit = DblLitIO;
  using cmp_lsp = CmpLspIO;
  using label = LabelIO;
  using str = StringIO;
  using key = KeyIO;

  std::istream& operator>>(std::istream&, sep&&);
  std::istream& operator>>(std::istream&, uncase_sep&&);
  std::istream& operator>>(std::istream&, dbl_lit&&);
  std::istream& operator>>(std::istream&, cmp_lsp&&);
  std::istream& operator>>(std::istream&, label&&);
  std::istream& operator>>(std::istream&, str&&);
  std::istream& operator>>(std::istream&, key&&);
  std::istream& operator>>(std::istream&, DataStruct&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);
  bool compare(const DataStruct&, const DataStruct&);
}

int main()
{
  std::vector< novikov::DataStruct > data;

  {
    using iit_t = std::istream_iterator< novikov::DataStruct >;
    std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));
  }

  {
    std::sort(std::begin(data), std::end(data), novikov::compare);
    using oit_t = std::ostream_iterator< novikov::DataStruct >;
    std::copy(std::begin(data), std::end(data), oit_t{std::cout, "\n"});
  }
  return 0;
}

std::istream& novikov::operator>>(std::istream& in, DataStruct& dest)
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

std::istream& novikov::operator>>(std::istream& in, key&& dest)
{
  std::istream::sentry s(in);
  if (!s || dest.used.empty())
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  DataType currentType = dest.used.back();

  int count = 0;
  for (auto t : dest.used) {
    if (t == currentType) count++;
  }
  if (count > 1 || currentType == Unknown)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  switch (currentType)
  {
    case DblLit:
      in >> dbl_lit{dest.input.key1};
      break;
    case CmpLsp:
      in >> cmp_lsp{dest.input.key2};
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

novikov::IOguard::IOguard(std::basic_ios< char >& s):
    s_(s),
    width_(s.width()),
    precision_(s.precision()),
    fmt_(s.flags()),
    fill_(s.fill())
{}

novikov::IOguard::~IOguard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

bool novikov::compare(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }

  double abs_lhs_k2 = std::abs(lhs.key2);
  double abs_rhs_k2 = std::abs(rhs.key2);
  if (abs_lhs_k2 != abs_rhs_k2)
  {
    return abs_lhs_k2 < abs_rhs_k2;
  }

  return lhs.key3.length() < rhs.key3.length();
}

std::istream& novikov::operator>>(std::istream& in, sep&& dest)
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

std::istream& novikov::operator>>(std::istream& in, uncase_sep&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char c = 0;
  if (in.get(c))
  {
    if (std::toupper(c) != std::toupper(dest.exp))
    {
      in.putback(c);
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::istream& novikov::operator>>(std::istream& in, label&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }

  std::string data;
  in << data;
  DataType inputType = DataType::Unknown;

  if (data == dest.possibleLabels[0])
  {
    inputType = DataType::DblLit;
  }
  else if (data == dest.possibleLabels[1])
  {
    inputType = DataType::CmpLsp;
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
