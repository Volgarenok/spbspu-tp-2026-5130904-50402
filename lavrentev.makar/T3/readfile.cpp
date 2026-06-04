#include <fstream>
#include "readfile.hpp"
#include "Figures.hpp"

void lavrentev::readfile(std::string name, std::vector< Polygon >& plgs)
{
  std::ifstream file(name);
  if (!file.is_open())
  {
    throw std::runtime_error("File open error");
  }

  std::copy(
    std::istream_iterator<Polygon>(file), 
    std::istream_iterator<Polygon>(), 
    std::back_inserter(plgs)
  );

  plgs.erase(
    std::remove_if(
      plgs.begin(),
      plgs.end(),
      std::bind(&lavrentev::Polygon::isEmpty, std::placeholders::_1)
    ), 
    plgs.end()
  );
}