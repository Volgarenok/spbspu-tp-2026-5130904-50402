#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include <string>
#include "DataStruct.hpp"

int main()
{
  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();
  using T = afanasev::DataStruct;
  using oit_t = std::ostream_iterator< T >;
  using iit_t = std::istream_iterator< T >;

  std::vector< T > data;

  while (!std::cin.eof())
  {
    std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));

    if (std::cin.fail())
    {
      std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      std::cin.ignore(streamMax, '\n');
    }
  }

  std::sort(data.begin(), data.end());

  std::copy(data.begin(), data.end(), oit_t{std::cout, "\n"});
}
