#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "DataStruct.hpp"

int main()
{
  using T = lavrentev::DataStruct;
  std::vector< T > data;
  using in_t = std::istream_iterator< T >;

  std::copy(in_t{std::cin}, in_t{}, std::back_inserter(data));
  std::sort(data.begin(), data.end());

  using out_t = std::ostream_iterator< T >;
  std::copy(data.begin(), data.end(), out_t{std::cout, "\n"});
}
