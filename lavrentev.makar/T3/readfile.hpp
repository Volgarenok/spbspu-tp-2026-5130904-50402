#ifndef READFILE_HPP
#define READFILE_HPP
#include <vector>
#include <istream>
#include "Figures.hpp"

namespace lavrentev
{
  void readfile(std::istream &in, std::vector< Polygon > &plgs);
}

#endif