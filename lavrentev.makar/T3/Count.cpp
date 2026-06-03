#include <iostream>
#include <iterator>
#include <map>
#include <algorithm>
#include "Figures.hpp"
#include "Count.hpp"

using namespace std::placeholders;

void lavrentev::count(std::istream& is, const std::vector< Polygon >& plgs)
{
  std::map< std::string, void (*)(std::istream&, const std::vector< Polygon >&) > cmds;
  cmds["EVEN"] = countEven;
  cmds["ODD"] = countOdd;
  std::string param;
  is >> param;
  if (cmds.find(param) != cmds.end())
  {
    cmds[param](is, plgs);
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
      countVrtxs(plgs, n);
    } else {
      throw std::invalid_argument("Invalid number");
    }
  }
  catch (...)
  {
    throw std::invalid_argument("Invalid command");
  }
}

void lavrentev::countEven(std::istream&, const std::vector< Polygon >& plgs)
{
  std::vector< Polygon > evenPlgs;
  std::copy_if(plgs.begin(), plgs.end(), std::back_inserter(evenPlgs), isEven);
  std::cout << evenPlgs.size() << "\n";
}

void lavrentev::countOdd(std::istream&, const std::vector< Polygon >& plgs)
{
  std::vector< Polygon > oddPlgs;
  std::copy_if(plgs.begin(), plgs.end(), std::back_inserter(oddPlgs), isOdd);
  std::cout << oddPlgs.size() << "\n";
}

void lavrentev::countVrtxs(const std::vector< Polygon >& plgs, size_t n)
{
  std::vector< Polygon > needPlgs;
  std::copy_if(
    plgs.begin(),
    plgs.end(),
    std::back_inserter(needPlgs),
    std::bind(isAmount, _1, n)
  );
  std::cout << needPlgs.size() << "\n";
}
