#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "DataStruct.hpp"

int main()
{
  using samarin::DataStruct;
  using input_iterator = std::istream_iterator< DataStruct >;

  std::vector< DataStruct > data;
  while (!std::cin.eof()) {
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(input_iterator(std::cin), input_iterator(), std::back_inserter(data));
  }

  std::sort(data.begin(), data.end(), samarin::compareData);
}
