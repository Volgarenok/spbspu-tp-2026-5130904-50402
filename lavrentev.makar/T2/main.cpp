#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>
#include "DataStruct.hpp"

int main()
{
  using T = lavrentev::DataStruct;
  std::vector< T > data;
  using in_t = std::istream_iterator< T >;

  while (std::cin) {
    std::copy(in_t{std::cin}, in_t{}, std::back_inserter(data));
    if (std::cin.eof())
    {
      break;
    }
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());

  using out_t = std::ostream_iterator< T >;
  std::copy(data.begin(), data.end(), out_t{std::cout, "\n"});
}
