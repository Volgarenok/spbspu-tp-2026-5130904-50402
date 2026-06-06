#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "commands.hpp"
#include "shapes.hpp"

int main(int argc, char** argv)
{
    if (argc != 2)
  {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }
  std::vector< petrov::Polygon > polygons;
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }

  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();
  using iit_t = std::istream_iterator< petrov::Polygon >;

  while (!file.eof())
  {
    std::copy(iit_t(file), iit_t(), std::back_inserter(polygons));
    if (!file)
    {
      file.clear();
      file.ignore(streamMax, '\n');
    }
  }
}
