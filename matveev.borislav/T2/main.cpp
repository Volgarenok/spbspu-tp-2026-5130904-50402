#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include "dataStruct.hpp"
#include "dataStructIO.hpp"

int main()
{
  std::vector< matveev::DataStruct > data;
  using input_it_t = std::istream_iterator< matveev::DataStruct >;

  while (!std::cin.eof())
  {
    std::copy(input_it_t{ std::cin }, input_it_t{}, std::back_inserter(data));

    if (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end(), matveev::compareDataStruct);

  using output_it_t = std::ostream_iterator< matveev::DataStruct >;
  std::copy(data.begin(), data.end(), output_it_t{ std::cout, "\n" });

  return 0;
}
