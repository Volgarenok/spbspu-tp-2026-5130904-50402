#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>
#include "io.hpp"

int main()
{
  std::vector< shirokov::DataStruct > data;

  {
    using iit_t = std::istream_iterator< shirokov::DataStruct >;

    while (!std::cin.eof())
    {
      std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));

      if (std::cin.fail() && !std::cin.eof())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }

  {
    std::sort(std::begin(data), std::end(data), shirokov::compare);
    using oit_t = std::ostream_iterator< shirokov::DataStruct >;
    std::copy(std::begin(data), std::end(data), oit_t{std::cout, "\n"});
  }
}
