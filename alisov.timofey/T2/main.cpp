#include <algorithm>
#include <iterator>
#include <limits>
#include <vector>
#include "DataStruct.hpp"

int main()
{
  std::vector< alisov::DataStruct > data;

  while (!std::cin.eof()) {
    std::copy(std::istream_iterator< alisov::DataStruct >(std::cin), std::istream_iterator< alisov::DataStruct >(),
              std::back_inserter(data));

    if (std::cin.fail() && !std::cin.eof()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());

  std::copy(data.cbegin(), data.cend(), std::ostream_iterator< alisov::DataStruct >(std::cout, "\n"));
}
