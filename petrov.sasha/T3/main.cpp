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

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }
}