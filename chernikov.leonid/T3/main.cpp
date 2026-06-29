#include "geometry.hpp"
#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: No input file specified\n";
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Error: Cannot open file '" << argv[1] << "'\n";
    return 1;
  }

  std::vector<chernikov::Polygon> polygons;

  std::copy(
      std::istream_iterator<chernikov::Polygon>(file),
      std::istream_iterator<chernikov::Polygon>(),
      std::back_inserter(polygons));

  file.close();

  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }
    chernikov::processCommand(line, polygons);
  }

  return 0;
}
