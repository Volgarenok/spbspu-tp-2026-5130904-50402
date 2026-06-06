#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "polygon.hpp"

namespace pozdnyakov
{
  void processArea(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void processMin(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void processMax(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void processCount(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void processPerms(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void processIntersections(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void readPolygons(std::istream &in, std::vector< Polygon > &polygons);

  struct CommandExecutor
  {
    const std::map< std::string, std::function< void(std::istream &, std::ostream &) > > &commands;

    bool operator()(const std::string &cmd) const;
  };

}

#endif
