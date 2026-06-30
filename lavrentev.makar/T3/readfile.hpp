#ifndef READFILE_HPP
#define READFILE_HPP
#include <vector>
#include "Figures.hpp"

namespace lavrentev
{
  void readfile(std::ifstream& file, std::vector< Polygon >& plgs);
}

#endif
