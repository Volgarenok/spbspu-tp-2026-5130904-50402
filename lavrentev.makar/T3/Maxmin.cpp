#include <iostream>
#include <map>
#include <algorithm>
#include <functional>
#include <iomanip>
#include "Figures.hpp"
#include "Maxmin.hpp"

using namespace std::placeholders;

void lavrentev::max(std::istream &is, const std::vector<Polygon> &plgs)
{
  std::map< std::string, void (*)(std::istream &, const std::vector<Polygon> &) > cmds;
  cmds["AREA"] = maxArea;
  cmds["VERTEXES"] = maxVrtxs;
  std::string param;
  is >> param;
  if (cmds.find(param) != cmds.end())
  {
    cmds[param](is, plgs);
    return;
  }
  else
  {
    throw std::invalid_argument("Invalid command");
  }
}

void lavrentev::maxArea(std::istream &, const std::vector<Polygon> &plgs)
{
  if (plgs.empty())
  {
    throw std::logic_error("No shapes for MAX");
  }
  auto mp = std::max_element(
    plgs.begin(),
    plgs.end(),
    std::bind(
      std::less<float>(),
      std::bind(&lavrentev::Polygon::getArea, _1),
      std::bind(&lavrentev::Polygon::getArea, _2)
    )
  );
  if (mp != plgs.end())
  {
    std::cout << std::fixed << std::setprecision(1) << (*mp).getArea() << "\n";
  }
}

void lavrentev::maxVrtxs(std::istream &, const std::vector<Polygon> &plgs)
{
  if (plgs.empty())
  {
    throw std::logic_error("No shapes for MAX");
  }
  auto mp = std::max_element(
    plgs.begin(),
    plgs.end(),
    std::bind(
      std::less<float>(),
      std::bind(&lavrentev::Polygon::getSize, _1),
      std::bind(&lavrentev::Polygon::getSize, _2)
    )
  );
  if (mp != plgs.end())
  {
    std::cout << (*mp).getSize() << "\n";
  }
}

void lavrentev::min(std::istream &is, const std::vector<Polygon> &plgs)
{
  std::map< std::string, void (*)(std::istream &, const std::vector<Polygon> &) > cmds;
  cmds["AREA"] = minArea;
  cmds["VERTEXES"] = minVrtxs;
  std::string param;
  is >> param;
  if (cmds.find(param) != cmds.end())
  {
    cmds[param](is, plgs);
    return;
  }
  else
  {
    throw std::invalid_argument("Invalid command");
  }
}

void lavrentev::minArea(std::istream &, const std::vector<Polygon> &plgs)
{
  if (plgs.empty())
  {
    throw std::logic_error("No shapes for MAX");
  }
  auto mp = std::min_element(
    plgs.begin(),
    plgs.end(),
    std::bind(
      std::less<float>(),
      std::bind(&lavrentev::Polygon::getArea, _1),
      std::bind(&lavrentev::Polygon::getArea, _2)
    )
  );
  if (mp != plgs.end())
  {
    std::cout << std::fixed << std::setprecision(1) << (*mp).getArea() << "\n";
  }
}

void lavrentev::minVrtxs(std::istream &, const std::vector<Polygon> &plgs)
{
  if (plgs.empty())
  {
    throw std::logic_error("No shapes for MAX");
  }
  auto mp = std::min_element(
    plgs.begin(),
    plgs.end(),
    std::bind(
      std::less<float>(),
      std::bind(&lavrentev::Polygon::getSize, _1),
      std::bind(&lavrentev::Polygon::getSize, _2)
    )
  );
  if (mp != plgs.end())
  {
    std::cout << (*mp).getSize() << "\n";
  }
}
