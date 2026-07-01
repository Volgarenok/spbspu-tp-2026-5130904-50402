#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "data.hpp"

int main()
{
  using DataStruct = pozdnyakov::DataStruct;
  using IStreamIterator = std::istream_iterator< DataStruct >;
  using OStreamIterator = std::ostream_iterator< DataStruct >;

  std::vector< DataStruct > parsedData;

  while (!std::cin.eof()) {
    IStreamIterator iter(std::cin);
    IStreamIterator end;
    std::copy(iter, end, std::back_inserter(parsedData));

    if (std::cin.fail() && !std::cin.eof()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(parsedData.begin(), parsedData.end());

  OStreamIterator outIter(std::cout, "\n");
  std::copy(parsedData.begin(), parsedData.end(), outIter);

  return 0;
}
