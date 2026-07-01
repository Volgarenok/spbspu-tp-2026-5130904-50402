#include <algorithm>
#include <forward_list>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include "data.hpp"
#include "sort.hpp"

int main()
{
  using DataStruct = pozdnyakov::DataStruct;
  using IStreamIterator = std::istream_iterator< DataStruct >;
  using OStreamIterator = std::ostream_iterator< DataStruct >;

  std::forward_list< DataStruct > parsedData;

  while (!std::cin.eof()) {
    IStreamIterator iter(std::cin);
    IStreamIterator end;
    std::copy(iter, end, std::front_inserter(parsedData));

    if (std::cin.fail() && !std::cin.eof()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  pozdnyakov::sort(parsedData.begin(), parsedData.end(), std::less< DataStruct >());

  OStreamIterator outIter(std::cout, "\n");
  std::copy(parsedData.begin(), parsedData.end(), outIter);

  return 0;
}
