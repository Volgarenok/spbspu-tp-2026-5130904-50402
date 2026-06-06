#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "utils.hpp"

int main()
{
  using data_t = saldaev::Data;
  std::vector< data_t > data;
  using iit_t = std::istream_iterator< data_t >;
  while (!std::cin.eof()) {
    std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));
    if (std::cin.fail()) {
      std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  using oit_t = std::ostream_iterator< data_t >;
  std::copy(data.begin(), data.end(), oit_t{std::cout, "\n"});
}
