#include "commands.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <istream>
#include <iterator>
#include <map>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <ioGuard.hpp>

#include "polygonAlgorithms.hpp"

namespace
{
  bool isEvenVertexes(const matveev::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOddVertexes(const matveev::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 1;
  }

  bool hasVertexCount(std::size_t count, const matveev::Polygon& polygon)
  {
    return polygon.points.size() == count;
  }

  bool isLessArea(const matveev::Polygon& lhs, const matveev::Polygon& rhs)
  {
    return matveev::getPolygonArea(lhs) < matveev::getPolygonArea(rhs);
  }

  bool isLessVertexes(const matveev::Polygon& lhs, const matveev::Polygon& rhs)
  {
    return lhs.points.size() < rhs.points.size();
  }

  bool isIntersectWith(const matveev::Polygon& target, const matveev::Polygon& polygon)
  {
    return matveev::isPolygonIntersect(target, polygon);
  }

  bool hasArea(double target, const matveev::Polygon& polygon)
  {
    return matveev::getPolygonArea(polygon) == target;
  }

  double getArea(const matveev::Polygon& polygon)
  {
    return matveev::getPolygonArea(polygon);
  }

  double getAreaSum(const matveev::data_t& data)
  {
    std::vector< double > areas;
    areas.reserve(data.size());

    std::transform(data.begin(), data.end(), std::back_inserter(areas), getArea);

    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  std::size_t readSize(const std::string& text)
  {
    std::size_t pos = 0;
    std::size_t value = std::stoull(text, &pos);

    if (pos != text.size())
    {
      throw std::logic_error("invalid number");
    }

    return value;
  }

  void printArea(std::ostream& out, double value)
  {
    matveev::IOGuard guard(out);
    out << std::fixed << std::setprecision(1) << value;
  }

  double areaEven(const matveev::data_t& data)
  {
    matveev::data_t selected;
    std::copy_if(data.begin(), data.end(), std::back_inserter(selected), isEvenVertexes);
    return getAreaSum(selected);
  }

  double areaOdd(const matveev::data_t& data)
  {
    matveev::data_t selected;
    std::copy_if(data.begin(), data.end(), std::back_inserter(selected), isOddVertexes);
    return getAreaSum(selected);
  }

  double areaMean(const matveev::data_t& data)
  {
    if (data.empty())
    {
      throw std::logic_error("empty data");
    }

    return getAreaSum(data) / data.size();
  }

  void printMaxArea(std::ostream& out, const matveev::data_t& data)
  {
    printArea(out, matveev::getPolygonArea(*std::max_element(data.begin(), data.end(), isLessArea)));
  }

  void printMaxVertexes(std::ostream& out, const matveev::data_t& data)
  {
    out << std::max_element(data.begin(), data.end(), isLessVertexes)->points.size();
  }

  void printMinArea(std::ostream& out, const matveev::data_t& data)
  {
    printArea(out, matveev::getPolygonArea(*std::min_element(data.begin(), data.end(), isLessArea)));
  }

  void printMinVertexes(std::ostream& out, const matveev::data_t& data)
  {
    out << std::min_element(data.begin(), data.end(), isLessVertexes)->points.size();
  }

  matveev::data_t filterEven(const matveev::data_t& source)
  {
    matveev::data_t result;
    std::copy_if(source.begin(), source.end(), std::back_inserter(result), isEvenVertexes);
    return result;
  }

  matveev::data_t filterOdd(const matveev::data_t& source)
  {
    matveev::data_t result;
    std::copy_if(source.begin(), source.end(), std::back_inserter(result), isOddVertexes);
    return result;
  }

  matveev::data_t filterVertexCount(const matveev::data_t& source, std::size_t count)
  {
    matveev::data_t result;
    using namespace std::placeholders;
    std::copy_if(source.begin(), source.end(), std::back_inserter(result), std::bind(hasVertexCount, count, _1));
    return result;
  }

  matveev::data_t filterMinArea(const matveev::data_t& source)
  {
    double value = matveev::getPolygonArea(*std::min_element(source.begin(), source.end(), isLessArea));
    matveev::data_t result;
    using namespace std::placeholders;
    std::copy_if(source.begin(), source.end(), std::back_inserter(result), std::bind(hasArea, value, _1));
    return result;
  }

  matveev::data_t filterMaxArea(const matveev::data_t& source)
  {
    double value = matveev::getPolygonArea(*std::max_element(source.begin(), source.end(), isLessArea));
    matveev::data_t result;
    using namespace std::placeholders;
    std::copy_if(source.begin(), source.end(), std::back_inserter(result), std::bind(hasArea, value, _1));
    return result;
  }

  bool restOfLineIsBlank(std::istream& in)
  {
    std::istream::int_type next = in.peek();

    while (next != std::char_traits< char >::eof() && next != '\n')
    {
      if (!std::isspace(next))
      {
        return false;
      }

      in.get();
      next = in.peek();
    }

    return true;
  }
}

matveev::Contexts::Contexts(const data_t& base):
  stack_(1, base)
{}

const matveev::data_t& matveev::Contexts::current() const
{
  return stack_.back();
}

std::size_t matveev::Contexts::level() const
{
  return stack_.size() - 1;
}

void matveev::Contexts::push(const data_t& context)
{
  stack_.push_back(context);
}

bool matveev::Contexts::pop()
{
  if (stack_.size() <= 1)
  {
    return false;
  }

  stack_.pop_back();
  return true;
}

bool matveev::doArea(std::istream& in, std::ostream& out, Contexts& contexts)
{
  const data_t& data = contexts.current();

  std::string arg;
  in >> arg;

  if (!in)
  {
    throw std::logic_error("missing argument");
  }

  if (!restOfLineIsBlank(in))
  {
    throw std::logic_error("trailing data");
  }

  std::map< std::string, std::function< double(const data_t&) > > subcommands;
  subcommands["EVEN"] = areaEven;
  subcommands["ODD"] = areaOdd;
  subcommands["MEAN"] = areaMean;

  if (subcommands.find(arg) != subcommands.end())
  {
    printArea(out, subcommands.at(arg)(data));
    return true;
  }

  std::size_t count = readSize(arg);

  if (count < 3)
  {
    throw std::logic_error("invalid vertex count");
  }

  data_t selected;
  using namespace std::placeholders;
  std::copy_if(data.begin(), data.end(), std::back_inserter(selected), std::bind(hasVertexCount, count, _1));
  printArea(out, getAreaSum(selected));
  return true;
}

bool matveev::doCount(std::istream& in, std::ostream& out, Contexts& contexts)
{
  const data_t& data = contexts.current();

  std::string arg;
  in >> arg;

  if (!in)
  {
    throw std::logic_error("missing argument");
  }

  if (!restOfLineIsBlank(in))
  {
    throw std::logic_error("trailing data");
  }

  std::map< std::string, std::function< bool(const Polygon&) > > predicates;
  predicates["EVEN"] = isEvenVertexes;
  predicates["ODD"] = isOddVertexes;

  if (predicates.find(arg) != predicates.end())
  {
    out << std::count_if(data.begin(), data.end(), predicates.at(arg));
    return true;
  }

  std::size_t count = readSize(arg);

  if (count < 3)
  {
    throw std::logic_error("invalid vertex count");
  }

  using namespace std::placeholders;
  out << std::count_if(data.begin(), data.end(), std::bind(hasVertexCount, count, _1));
  return true;
}

bool matveev::doMax(std::istream& in, std::ostream& out, Contexts& contexts)
{
  const data_t& data = contexts.current();

  if (data.empty())
  {
    throw std::logic_error("empty data");
  }

  std::string arg;
  in >> arg;

  if (!in)
  {
    throw std::logic_error("missing argument");
  }

  if (!restOfLineIsBlank(in))
  {
    throw std::logic_error("trailing data");
  }

  std::map< std::string, std::function< void(std::ostream&, const data_t&) > > subcommands;
  subcommands["AREA"] = printMaxArea;
  subcommands["VERTEXES"] = printMaxVertexes;

  if (subcommands.find(arg) == subcommands.end())
  {
    throw std::logic_error("invalid max command");
  }

  subcommands.at(arg)(out, data);
  return true;
}

bool matveev::doMin(std::istream& in, std::ostream& out, Contexts& contexts)
{
  const data_t& data = contexts.current();

  if (data.empty())
  {
    throw std::logic_error("empty data");
  }

  std::string arg;
  in >> arg;

  if (!in)
  {
    throw std::logic_error("missing argument");
  }

  if (!restOfLineIsBlank(in))
  {
    throw std::logic_error("trailing data");
  }

  std::map< std::string, std::function< void(std::ostream&, const data_t&) > > subcommands;
  subcommands["AREA"] = printMinArea;
  subcommands["VERTEXES"] = printMinVertexes;

  if (subcommands.find(arg) == subcommands.end())
  {
    throw std::logic_error("invalid min command");
  }

  subcommands.at(arg)(out, data);
  return true;
}

bool matveev::doInFrame(std::istream& in, std::ostream& out, Contexts& contexts)
{
  const data_t& data = contexts.current();

  if (data.empty())
  {
    throw std::logic_error("empty data");
  }

  Polygon polygon;
  in >> polygon;

  if (!in)
  {
    throw std::logic_error("invalid polygon");
  }

  if (!restOfLineIsBlank(in))
  {
    throw std::logic_error("trailing data");
  }

  Frame frame = getFrame(data);

  if (isPolygonInFrame(frame, polygon))
  {
    out << "<TRUE>";
  }
  else
  {
    out << "<FALSE>";
  }

  return true;
}

bool matveev::doIntersections(std::istream& in, std::ostream& out, Contexts& contexts)
{
  const data_t& data = contexts.current();

  Polygon polygon;
  in >> polygon;

  if (!in)
  {
    throw std::logic_error("invalid polygon");
  }

  if (!restOfLineIsBlank(in))
  {
    throw std::logic_error("trailing data");
  }

  using namespace std::placeholders;
  out << std::count_if(data.begin(), data.end(), std::bind(isIntersectWith, polygon, _1));
  return true;
}

bool matveev::doContext(std::istream& in, std::ostream& out, Contexts& contexts)
{
  std::string arg;
  in >> arg;

  if (!in)
  {
    throw std::logic_error("missing argument");
  }

  if (!restOfLineIsBlank(in))
  {
    throw std::logic_error("trailing data");
  }

  const data_t& base = contexts.current();

  if (base.empty())
  {
    throw std::logic_error("empty context");
  }

  std::map< std::string, std::function< data_t(const data_t&) > > filters;
  filters["MIN-AREA"] = filterMinArea;
  filters["MAX-AREA"] = filterMaxArea;
  filters["EVEN"] = filterEven;
  filters["ODD"] = filterOdd;

  data_t filtered;

  if (filters.find(arg) != filters.end())
  {
    filtered = filters.at(arg)(base);
  }
  else
  {
    std::size_t count = readSize(arg);
    filtered = filterVertexCount(base, count);
  }

  contexts.push(filtered);

  if (filtered.empty())
  {
    out << "<EMPTY CONTEXT>";
    return true;
  }

  return false;
}

bool matveev::doPopContext(std::istream& in, std::ostream& out, Contexts& contexts)
{
  if (!restOfLineIsBlank(in))
  {
    throw std::logic_error("trailing data");
  }

  if (!contexts.pop())
  {
    out << "<COMMON CONTEXT>";
    return true;
  }

  return false;
}

bool matveev::doLevel(std::istream& in, std::ostream& out, Contexts& contexts)
{
  if (!restOfLineIsBlank(in))
  {
    throw std::logic_error("trailing data");
  }

  out << "<LEVEL: " << contexts.level() << ">";
  return true;
}
