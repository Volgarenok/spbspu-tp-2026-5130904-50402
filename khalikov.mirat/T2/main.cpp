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

int main()
{
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
