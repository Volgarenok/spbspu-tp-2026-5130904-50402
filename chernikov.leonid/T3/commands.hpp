#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "geometry.hpp"
#include <vector>
#include <string>

namespace chernikov
{

  void processCommand(const std::string &line, std::vector<Polygon> &polygons);

}

#endif
