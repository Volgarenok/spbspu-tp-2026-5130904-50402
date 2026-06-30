#include "readfile.hpp"
#include <fstream>
#include <limits>
#include <vector>
#include "Figures.hpp"

void lavrentev::readfile(std::ifstream& file, std::vector< Polygon >& plgs)
{
  while (!file.eof())
  {
    Polygon plg;
    if (file >> plg)
    {
      plgs.push_back(plg);
    }
    else
    {
      if (file.eof())
      {
        break;
      }
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}