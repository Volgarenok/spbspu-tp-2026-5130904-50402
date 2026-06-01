#include <cstddef>
#include <map>
#include "Figures.hpp"

void area(std::istream& is, std::vector< Polygon > plgs)
{
  std::map< std::string, void (*)(std::vector< Polygon >) > cmds;
  cmds["EVEN"] = areaEven;
  cmds["ODD"] = areaOdd;
  cmds["MEAN"] = areaMean;
  std::string param;
  is >> param;
  if (cmds.find(param) != cmds.end())
  {
    cmds[param](plgs);
    return;
  }
  try
  {
    if (param[0] == '-') {
      throw std::invalid_argument("Negative number");
    }
    size_t pos;

    unsigned long long result = std::stoull(param, &pos);
    if (pos == param.length()) {
      size_t n = static_cast<size_t>(result);
      areaVrtxs(plgs, n);
    } else {
      throw std::invalid_argument("Invalid number");
    }
  }
  catch (...)
  {
    throw std::invalid_argument("Invalid number");
  }
}


