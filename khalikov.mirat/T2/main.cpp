#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <iomanip>

//DBL LIT
//ULL OCT
namespace khalikov {

	struct DataStruct {
	  double key1;
	  unsigned long long key2;
	  std::string key3;
	};

  struct DelimiterIO {
    char exp;
  };

  struct DoubleIO {
    double& ref;
  };

  struct UllIO {
    unsigned long long& ref;
  };

  struct StringIO {
    std::string& ref;
  };

  enum class Key {
    KEY1,
    KEY2,
    KEY3
  };

  struct KeyIO {
    Key& ref;
  };

  class IoGuard {
    public:
      explicit IoGuard(std::basic_ios< char >& s);
      ~IoGuard();
    private:
      std::basic_ios< char >& s_;
      char fill_;
      std::streamsize precision_;
      std::basic_ios< char >::fmtflags fmt_;
   };

  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, UllIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, KeyIO&& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
}

int main() {
  std::vector< khalikov::DataStruct > data;
  {
    using iit_t = std::istream_iterator< khalikov::DataStruct >;
    while (!std::cin.eof()) {
      std::copy(iit_t(std::cin), iit_t(), std::back_inserter(data))
      if (std::cin.fail() && !std::cin.eof()) {
        std::cin.clear();
        auto toIgnore = std::numeric_limits< std::streamsize >::max();
        std::cin.ignore(toIgnore, '\n');
      }
    }
  }
  std::sort(data.begin(), data.end());
  {
    using oit_t = std::ostream_iterator<khalikov::DataStruct>;
    std::copy(data.begin(), data.end(), oit_t(std::cout, '\n'));
  }
}




