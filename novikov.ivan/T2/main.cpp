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
    String
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
