#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include <IOGuard.hpp>
#include <DelimiterIO.hpp>
#include "DataStruct.hpp"
#include "DataTypes.hpp"
#include "LabelIO.hpp"
#include "KeyIO.hpp"

int main()
{
  std::vector< novikov::DataStruct > data;

  {
    using iit_t = std::istream_iterator< novikov::DataStruct >;
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
    std::sort(std::begin(data), std::end(data), novikov::compare);
    using oit_t = std::ostream_iterator< novikov::DataStruct >;
    std::copy(std::begin(data), std::end(data), oit_t{std::cout, "\n"});
  }
  return 0;
}
