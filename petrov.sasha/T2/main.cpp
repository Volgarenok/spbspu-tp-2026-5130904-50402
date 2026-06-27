#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include "utils.hpp"

int main()
{
  using T = petrov::DataStruct;
  using iit_t = std::istream_iterator< T >;
  using oit_t = std::ostream_iterator< T >;
  std::vector< T > data;

  while (!std::cin.eof()) {
    std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));
    if (std::cin.fail()) {
      std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), oit_t{std::cout, "\n"});
}

