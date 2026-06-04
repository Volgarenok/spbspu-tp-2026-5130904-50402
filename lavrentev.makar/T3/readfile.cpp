#include <fstream>
#include <algorithm>
#include <iterator>
#include <functional>
#include "readfile.hpp"
#include "Figures.hpp"

void lavrentev::readfile(std::string name, std::vector<Polygon>& plgs)
{
  std::ifstream file(name);
  if (!file)
  {
    throw std::runtime_error("File open error");
  }

  std::vector<Polygon> temp{
    std::istream_iterator<Polygon>(file),
    std::istream_iterator<Polygon>()
  };

  std::remove_copy_if(
    temp.begin(),
    temp.end(),
    std::back_inserter(plgs),
    std::mem_fn(&Polygon::isEmpty)
  );
}
