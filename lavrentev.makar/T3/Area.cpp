#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <functional>
#include "Area.hpp"
#include "Figures.hpp"

using namespace std::placeholders;

void lavrentev::area(std::istream &is, const std::vector<Polygon> &plgs)
{
  std::map< std::string, void (*)(std::istream &, const std::vector<Polygon> &) > cmds;
  cmds["EVEN"] = areaEven;
  cmds["ODD"] = areaOdd;
  cmds["MEAN"] = areaMean;
  std::string param;
  is >> param;
  if (cmds.find(param) != cmds.end())
  {
    cmds[param](is, plgs);
    return;
  }
  try
  {
    if (param[0] == '-')
    {
      throw std::invalid_argument("Negative number");
    }
    size_t pos;

    unsigned long long result = std::stoull(param, &pos);
    if (pos == param.length())
    {
      size_t n = static_cast<size_t>(result);
      if (n < 3)
      {
        throw std::invalid_argument("Polygon has 3 or more points");
      }
      areaVrtxs(plgs, n);
    }
    else
    {
      throw std::invalid_argument("Invalid number");
    }
  } catch (...)
  {
    throw std::invalid_argument("Invalid command");
  }
}

void lavrentev::areaEven(std::istream &, const std::vector<Polygon> &plgs)
{
  std::vector<Polygon> evenPlgs;
  std::copy_if(plgs.begin(), plgs.end(), std::back_inserter(evenPlgs), isEven);
  std::vector<float> areas(evenPlgs.size());
  std::transform(
    evenPlgs.begin(),
    evenPlgs.end(),
    areas.begin(),
    std::bind(&lavrentev::Polygon::getArea, _1)
  );
  float total = std::accumulate(areas.begin(), areas.end(), 0.0f);
  std::cout << std::fixed << std::setprecision(1) << total << "\n";
}

void lavrentev::areaOdd(std::istream &, const std::vector<Polygon> &plgs)
{
  std::vector<Polygon> oddPlgs;
  std::copy_if(plgs.begin(), plgs.end(), std::back_inserter(oddPlgs), isOdd);
  std::vector<float> areas(oddPlgs.size());
  std::transform(
    oddPlgs.begin(),
    oddPlgs.end(),
    areas.begin(),
    std::bind(&lavrentev::Polygon::getArea, _1)
  );
  float total = std::accumulate(areas.begin(), areas.end(), 0.0f);
  std::cout << std::fixed << std::setprecision(1) << total << "\n";
}

void lavrentev::areaMean(std::istream &, const std::vector<Polygon> &plgs)
{
  if (!plgs.size())
  {
    return;
  }
  std::vector<float> areas(plgs.size());
  std::transform(
    plgs.begin(),
    plgs.end(),
    areas.begin(),
    std::bind(&lavrentev::Polygon::getArea, _1)
  );
  float total = std::accumulate(areas.begin(), areas.end(), 0.0f);
  std::cout << std::fixed << std::setprecision(1) << total / areas.size() << "\n";
}

void lavrentev::areaVrtxs(const std::vector<Polygon> &plgs, size_t n)
{
  std::vector<Polygon> needPlgs;
  std::copy_if(
    plgs.begin(),
    plgs.end(),
    std::back_inserter(needPlgs),
    std::bind(isAmount, _1, n)
  );
  if (!needPlgs.size())
  {
    return;
  }
  std::vector<float> areas(needPlgs.size());
  std::transform(
    needPlgs.begin(),
    needPlgs.end(),
    areas.begin(),
    std::bind(&lavrentev::Polygon::getArea, _1)
  );
  float total = std::accumulate(areas.begin(), areas.end(), 0.0f);
  std::cout << std::fixed << std::setprecision(1) << total << "\n";
}
