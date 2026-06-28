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
  using PolygonList = std::vector< Polygon >;
  using Predicate = std::function< bool(const Polygon &) >;
  using Handler = std::function< void(const PolygonList &, std::istream &, std::ostream &) >;

  int skipInlineBlanks(std::istream &in)
  {
    int symbol = in.peek();
    while ((symbol == ' ') || (symbol == '\t')) {
      in.get();
      symbol = in.peek();
    }
    return symbol;
  }

  bool atLineEnd(int symbol)
  {
    return (symbol == '\n') || (symbol == std::char_traits< char >::eof());
  }

  std::string readWord(std::istream &in)
  {
    if (atLineEnd(skipInlineBlanks(in))) {
      throw std::invalid_argument("expected argument");
    }
    std::string word;
    in >> word;
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

  std::size_t vertexCount(const Polygon &polygon)
  {
    return polygon.points.size();
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

  std::vector< double > areasOf(const PolygonList &polygons)
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), samarin::getArea);
    return areas;
  }

  std::vector< std::size_t > vertexCountsOf(const PolygonList &polygons)
  {
    std::vector< std::size_t > counts(polygons.size());
    std::transform(polygons.begin(), polygons.end(), counts.begin(), vertexCount);
    return counts;
  }

  double sumAreaWhere(const PolygonList &polygons, const Predicate &pred)
  {
    PolygonList matched;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(matched), pred);
    const std::vector< double > areas = areasOf(matched);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  Predicate vertexPredicate(const std::string &param)
  {
    using namespace std::placeholders;
    if (param == "EVEN") {
      return hasEvenVertices;
    }
    if (param == "ODD") {
      return hasOddVertices;
    }
    return std::bind(hasVertexCount, _1, parseVertexCount(param));
  }

  void commandArea(const PolygonList &polygons, std::istream &in, std::ostream &out)
  {
    const std::string param = readWord(in);
    double result = 0.0;
    if (param == "MEAN") {
      if (polygons.empty()) {
        throw std::invalid_argument("no polygons for mean");
      }
      const std::vector< double > areas = areasOf(polygons);
      const double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      result = total / static_cast< double >(polygons.size());
    } else {
      result = sumAreaWhere(polygons, vertexPredicate(param));
    }
    out << result << '\n';
  }

  void commandExtremum(const PolygonList &polygons, std::istream &in, std::ostream &out,
      bool useMax)
  {
    if (polygons.empty()) {
      throw std::invalid_argument("no polygons");
    }
    const std::string param = readWord(in);
    if (param == "AREA") {
      const std::vector< double > areas = areasOf(polygons);
      const double value = useMax ? *std::max_element(areas.begin(), areas.end())
        : *std::min_element(areas.begin(), areas.end());
      out << value << '\n';
    } else if (param == "VERTEXES") {
      const std::vector< std::size_t > counts = vertexCountsOf(polygons);
      const std::size_t value = useMax ? *std::max_element(counts.begin(), counts.end())
        : *std::min_element(counts.begin(), counts.end());
      out << value << '\n';
    } else {
      throw std::invalid_argument("bad extremum parameter");
    }
  }

  void commandCount(const PolygonList &polygons, std::istream &in, std::ostream &out)
  {
    const auto pred = vertexPredicate(readWord(in));
    const auto matched = std::count_if(polygons.begin(), polygons.end(), pred);
    out << static_cast< std::size_t >(matched) << '\n';
  }

  void commandRightShapes(const PolygonList &polygons, std::istream &, std::ostream &out)
  {
    const auto count = std::count_if(polygons.begin(), polygons.end(), samarin::hasRightAngle);
    out << static_cast< std::size_t >(count) << '\n';
  }

  void commandIntersections(const PolygonList &polygons, std::istream &in, std::ostream &out)
  {
    using namespace std::placeholders;
    if (atLineEnd(skipInlineBlanks(in))) {
      throw std::invalid_argument("expected a polygon");
    }
    Polygon query;
    if (!(in >> query)) {
      throw std::invalid_argument("expected a polygon");
    }
    const auto matches = std::bind(samarin::intersects, _1, query);
    const auto count = std::count_if(polygons.begin(), polygons.end(), matches);
    out << static_cast< std::size_t >(count) << '\n';
  }

  std::map< std::string, Handler > makeHandlers()
  {
    using namespace std::placeholders;
    std::map< std::string, Handler > handlers;
    handlers["AREA"] = commandArea;
    handlers["MAX"] = std::bind(commandExtremum, _1, _2, _3, true);
    handlers["MIN"] = std::bind(commandExtremum, _1, _2, _3, false);
    handlers["COUNT"] = commandCount;
    handlers["RIGHTSHAPES"] = commandRightShapes;
    handlers["INTERSECTIONS"] = commandIntersections;
    return handlers;
  }
}

void samarin::processCommands(const PolygonList &polygons, std::istream &in, std::ostream &out)
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
