#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "Polygon.hpp"

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cerr << "ERROR: expected a single file name argument\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "ERROR: cannot open the input file\n";
    return 2;
  }

  using samarin::Polygon;
  using input_iterator = std::istream_iterator< Polygon >;
  std::vector< Polygon > polygons;
  while (!file.eof()) {
    if (file.fail()) {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(input_iterator(file), input_iterator(), std::back_inserter(polygons));
  }
}
