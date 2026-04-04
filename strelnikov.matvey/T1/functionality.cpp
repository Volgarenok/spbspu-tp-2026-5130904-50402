#include <string>
#include <vector>
#include "functionality.hpp"

struct strelnikov::Note
{
  Note(std::string);
  std::string name;
  std::vector< std::string > lines;
  std::vector< std::weak_ptr< Note > > links;
};