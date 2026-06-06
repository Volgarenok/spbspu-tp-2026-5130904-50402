#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include "commands.hpp"
#include "polygon.hpp"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  std::vector< pozdnyakov::Polygon > polygons{};
  pozdnyakov::readPolygons(file, polygons);

  const std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands = {
      {"AREA", std::bind(pozdnyakov::processArea, std::placeholders::_1, std::placeholders::_2, std::cref(polygons))},
      {"MAX", std::bind(pozdnyakov::processMax, std::placeholders::_1, std::placeholders::_2, std::cref(polygons))},
      {"MIN", std::bind(pozdnyakov::processMin, std::placeholders::_1, std::placeholders::_2, std::cref(polygons))},
      {"COUNT", std::bind(pozdnyakov::processCount, std::placeholders::_1, std::placeholders::_2, std::cref(polygons))},
      {"PERMS", std::bind(pozdnyakov::processPerms, std::placeholders::_1, std::placeholders::_2, std::cref(polygons))},
      {"INTERSECTIONS", std::bind(pozdnyakov::processIntersections, std::placeholders::_1, std::placeholders::_2,
        std::cref(polygons))}};

  std::cout << std::fixed << std::setprecision(1);

  const bool result = std::any_of(std::istream_iterator< std::string >(std::cin),
                                  std::istream_iterator< std::string >(), pozdnyakov::CommandExecutor{commands});
  static_cast< void >(result);

  return 0;

}
