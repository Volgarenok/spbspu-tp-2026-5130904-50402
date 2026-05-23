#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "data.hpp"

int main()
{
  std::vector< pozdnyakov::DataStruct > data;

  while (!std::cin.eof()) {
    std::copy(std::istream_iterator< pozdnyakov::DataStruct >(std::cin),
              std::istream_iterator< pozdnyakov::DataStruct >(), std::back_inserter(data));

    if (std::cin.fail() && !std::cin.eof()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator< pozdnyakov::DataStruct >(std::cout, "\n"));

  return 0;
}
