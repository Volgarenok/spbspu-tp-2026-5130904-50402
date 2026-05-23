#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include "DataStruct.hpp"

int main()
{
  std::vector<sogdanov::DataStruct> data;

  while (!std::cin.eof()) {
    std::copy(
      std::istream_iterator<sogdanov::DataStruct>(std::cin),
      std::istream_iterator<sogdanov::DataStruct>(),
      std::back_inserter(data)
    );

    if (std::cin.fail() && !std::cin.eof()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());

  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator<sogdanov::DataStruct>(std::cout, "\n")
  );
}
