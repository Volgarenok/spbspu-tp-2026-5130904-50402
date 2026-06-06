#include "commands.hpp"
#include <algorithm>
#include <iterator>
#include <limits>
#include <stdexcept>

namespace pozdnyakov
{

  namespace
  {
    struct IsDigit
    {
      bool operator()(char c) const
      {
        return std::isdigit(static_cast< unsigned char >(c));
      }
    };

    bool isNumericString(const std::string &str)
    {
      if (str.empty()) {
        return false;
      }
      return std::all_of(str.begin(), str.end(), IsDigit{});
    }
  }

  void processArea(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string arg = "";
    in >> arg;

    if (arg == "EVEN") {
      const double result = calculateAreaEven(polygons);
      out << result << '\n';
    } else if (arg == "ODD") {
      const double result = calculateAreaOdd(polygons);
      out << result << '\n';
    } else if (arg == "MEAN") {
      const double result = calculateAreaMean(polygons);
      out << result << '\n';
    } else if (isNumericString(arg)) {
      const size_t num = std::stoull(arg);
      const double result = calculateAreaNum(polygons, num);
      out << result << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  void processMin(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string arg = "";
    in >> arg;

    if (arg == "AREA") {
      const double result = getMinArea(polygons);
      out << result << '\n';
    } else if (arg == "VERTEXES") {
      const size_t result = getMinVertexes(polygons);
      out << result << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  void processMax(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string arg = "";
    in >> arg;

    if (arg == "AREA") {
      const double result = getMaxArea(polygons);
      out << result << '\n';
    } else if (arg == "VERTEXES") {
      const size_t result = getMaxVertexes(polygons);
      out << result << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  void processCount(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string arg = "";
    in >> arg;

    if (arg == "EVEN") {
      const size_t result = countEvenVertices(polygons);
      out << result << '\n';
    } else if (arg == "ODD") {
      const size_t result = countOddVertices(polygons);
      out << result << '\n';
    } else if (isNumericString(arg)) {
      const size_t num = std::stoull(arg);
      const size_t result = countNumVertices(polygons, num);
      out << result << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  void processPerms(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    Polygon target{};
    if (in >> target) {
      const size_t result = countPermutations(polygons, target);
      out << result << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  void processIntersections(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    Polygon target{};
    if (in >> target) {
      const size_t result = countIntersections(polygons, target);
      out << result << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  void readPolygons(std::istream &in, std::vector< Polygon > &polygons)
  {
    std::copy(std::istream_iterator< Polygon >(in), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
    if (in.fail() && !in.eof()) {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      readPolygons(in, polygons);
    }
  }

  bool CommandExecutor::operator()(const std::string &cmd) const
  {
    try {
      commands.at(cmd)(std::cin, std::cout);
    } catch (const std::exception &) {
      if (std::cin.fail()) {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
    return false;
  }

}
