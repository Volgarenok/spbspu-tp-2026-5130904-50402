#include "readfile.hpp"
#include <algorithm>
#include <iterator>
#include <functional>
#include "Figures.hpp"

void lavrentev::readfile(std::istream &in, std::vector< Polygon >& plgs)
{
  std::vector< Polygon > temp{
    std::istream_iterator< Polygon >(in),
    std::istream_iterator< Polygon >()
  };

  std::remove_copy_if(
    temp.begin(),
    temp.end(),
    std::back_inserter(plgs),
    std::mem_fn(&Polygon::isEmpty)
  );
}
