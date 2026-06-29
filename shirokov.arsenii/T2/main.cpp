#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include "io.hpp"

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
