#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "DataStruct.hpp"

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
