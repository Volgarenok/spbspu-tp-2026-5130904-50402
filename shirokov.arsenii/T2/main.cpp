#include <algorithm>
#include <iostream>
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
    String
  };

  struct UllOctIO
  {
    ull& ref;
  };

  struct UllBinIO
  {
    ull& ref;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct LabelIO
  {
    std::string exp;
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

  std::istream& operator>>(std::istream&, DelimiterIO&&);
  std::istream& operator>>(std::istream&, UllOctIO&&);
  std::istream& operator>>(std::istream&, UllBinIO&&);
  std::istream& operator>>(std::istream&, LabelIO&&);
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
