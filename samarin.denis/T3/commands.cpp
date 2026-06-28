#include "commands.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <istream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "StreamGuard.hpp"

namespace {
  using samarin::Polygon;
  using Handler = std::function< void(const std::vector< Polygon > &, std::istream &, std::ostream &) >;

  std::string readWord(std::istream &in)
  {
    std::string word;
    if (!(in >> word)) {
      throw std::invalid_argument("expected argument");
    }
    return word;
  }

  bool hasEvenVertices(const Polygon &polygon)
  {
    return (polygon.points.size() % 2) == 0;
  }

  bool hasOddVertices(const Polygon &polygon)
  {
    return (polygon.points.size() % 2) != 0;
  }

  bool hasVertexCount(const Polygon &polygon, std::size_t count)
  {
    return polygon.points.size() == count;
  }

  std::size_t parseVertexCount(const std::string &token)
  {
    const auto isDigit = [](char symbol)
    {
      return std::isdigit(static_cast< unsigned char >(symbol)) != 0;
    };
    if (token.empty() || !std::all_of(token.begin(), token.end(), isDigit)) {
      throw std::invalid_argument("not a vertex count");
    }
    const unsigned long long value = std::stoull(token);
    if (value < 3) {
      throw std::invalid_argument("vertex count is too small");
    }
    return static_cast< std::size_t >(value);
  }

  std::vector< double > areasOf(const std::vector< Polygon > &polygons)
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), samarin::getArea);
    return areas;
  }

  double sumAreaWhere(const std::vector< Polygon > &polygons,
      const std::function< bool(const Polygon &) > &pred)
  {
    std::vector< Polygon > matched;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(matched), pred);
    const std::vector< double > areas = areasOf(matched);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  void commandArea(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out)
  {
    using namespace std::placeholders;
    const std::string param = readWord(in);
    double result = 0.0;
    if (param == "EVEN") {
      result = sumAreaWhere(polygons, hasEvenVertices);
    } else if (param == "ODD") {
      result = sumAreaWhere(polygons, hasOddVertices);
    } else if (param == "MEAN") {
      if (polygons.empty()) {
        throw std::invalid_argument("no polygons for mean");
      }
      const std::vector< double > areas = areasOf(polygons);
      const double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      result = total / static_cast< double >(polygons.size());
    } else {
      const std::size_t count = parseVertexCount(param);
      result = sumAreaWhere(polygons, std::bind(hasVertexCount, _1, count));
    }
    out << result << '\n';
  }

  void commandCount(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out)
  {
    using namespace std::placeholders;
    const std::string param = readWord(in);
    std::size_t result = 0;
    if (param == "EVEN") {
      result = static_cast< std::size_t >(std::count_if(polygons.begin(), polygons.end(), hasEvenVertices));
    } else if (param == "ODD") {
      result = static_cast< std::size_t >(std::count_if(polygons.begin(), polygons.end(), hasOddVertices));
    } else {
      const std::size_t count = parseVertexCount(param);
      const auto matches = std::bind(hasVertexCount, _1, count);
      result = static_cast< std::size_t >(std::count_if(polygons.begin(), polygons.end(), matches));
    }
    out << result << '\n';
  }

  std::map< std::string, Handler > makeHandlers()
  {
    std::map< std::string, Handler > handlers;
    handlers["AREA"] = commandArea;
    handlers["COUNT"] = commandCount;
    return handlers;
  }
}

void samarin::processCommands(const std::vector< Polygon > &polygons, std::istream &in, std::ostream &out)
{
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);
  const std::map< std::string, Handler > handlers = makeHandlers();
  std::string command;
  while (in >> command) {
    try {
      const auto found = handlers.find(command);
      if (found == handlers.end()) {
        throw std::invalid_argument("unknown command");
      }
      found->second(polygons, in, out);
    } catch (const std::exception &) {
      if (in.fail() && !in.eof()) {
        in.clear();
      }
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      out << "<INVALID COMMAND>\n";
    }
  }
}
