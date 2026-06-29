#include "data_struct.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

int main()
{
  std::vector<chernikov::DataStruct> data_vector;

  std::copy(
      std::istream_iterator<chernikov::DataStruct>(std::cin),
      std::istream_iterator<chernikov::DataStruct>(),
      std::back_inserter(data_vector));

  if (!std::cin.eof())
  {
    std::cerr << "Input error\n";
    return 1;
  }

  std::sort(data_vector.begin(), data_vector.end(), chernikov::compare_data);

  std::copy(
      data_vector.begin(),
      data_vector.end(),
      std::ostream_iterator<chernikov::DataStruct>(std::cout, "\n"));

  return 0;
}
