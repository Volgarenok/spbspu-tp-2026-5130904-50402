#include "commands.hpp"

#include <functional>
#include <iomanip>
#include <istream>
#include <limits>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "StreamGuard.hpp"

namespace {
  using samarin::Polygon;
  using Handler = std::function< void(const std::vector< Polygon > &, std::istream &, std::ostream &) >;

  std::map< std::string, Handler > makeHandlers()
  {
    std::map< std::string, Handler > handlers;
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
