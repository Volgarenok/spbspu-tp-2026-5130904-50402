#include "Commands.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <functional>
#include <iomanip>
#include <map>
#include <numeric>
#include <stdexcept>
#include <string>
#include "../common/IoGuard.hpp"
#include "IoUtils.hpp"

namespace sogdanov {
  namespace {

    struct CheckNotSpace {
      bool operator()(char c) const
      {
        return !std::isspace(static_cast< unsigned char >(c));
      }
    };

    void checkTrailingGarbage(std::istream& in)
    {
      std::string rest;
      if (std::getline(in, rest)) {
        if (!in.eof()) {
          in.putback('\n');
        }
        if (std::any_of(rest.begin(), rest.end(), CheckNotSpace())) {
          throw std::logic_error("Trailing garbage");
        }
      }
    }

    struct MultiplyCross {
      double operator()(const Point& a, const Point& b) const
      {
        return static_cast< double >((a.x * b.y) - (a.y * b.x));
      }
    };

    double getArea(const Polygon& p)
    {
      std::vector< double > crossProducts;
      std::transform(p.points.begin(), p.points.end() - 1, p.points.begin() + 1,
        std::back_inserter(crossProducts), MultiplyCross());
      crossProducts.push_back(MultiplyCross()(p.points.back(), p.points.front()));
      const double sum = std::accumulate(crossProducts.begin(), crossProducts.end(), 0.0);
      return std::abs(sum) / 2.0;
    }

    size_t getVerticesCount(const Polygon& p)
    {
      return p.points.size();
    }

    bool isEven(const Polygon& p)
    {
      return (p.points.size() % 2) == 0;
    }

    bool isOdd(const Polygon& p)
    {
      return (p.points.size() % 2) != 0;
    }

    bool hasVertices(const Polygon& p, size_t n)
    {
      return p.points.size() == n;
    }

    bool isPermutation(const Polygon& p, const Polygon& target)
    {
      if (p.points.size() != target.points.size()) {
        return false;
      }
      return std::is_permutation(p.points.begin(), p.points.end(), target.points.begin());
    }

    double sumArea(const std::vector< Polygon >& polygons)
    {
      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);
      return std::accumulate(areas.begin(), areas.end(), 0.0);
    }

    size_t countMaxSeq(std::vector< Polygon >::const_iterator first,
      std::vector< Polygon >::const_iterator last,
      const Polygon& target,
      size_t currentMax)
    {
      const auto start = std::find(first, last, target);
      if (start == last) {
        return currentMax;
      }
      const auto end = std::find_if_not(start, last,
        std::bind(std::equal_to< Polygon >(), std::placeholders::_1, target));
      const size_t seqLen = static_cast< size_t >(std::distance(start, end));
      return countMaxSeq(end, last, target, std::max(currentMax, seqLen));
    }

    void areaEven(std::ostream& out, const std::vector< Polygon >& polygons)
    {
      std::vector< Polygon > filtered;
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isEven);
      IoGuard guard(out);
      out << std::fixed << std::setprecision(1) << sumArea(filtered);
    }

    void areaOdd(std::ostream& out, const std::vector< Polygon >& polygons)
    {
      std::vector< Polygon > filtered;
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isOdd);
      IoGuard guard(out);
      out << std::fixed << std::setprecision(1) << sumArea(filtered);
    }

    void areaMean(std::ostream& out, const std::vector< Polygon >& polygons)
    {
      if (polygons.empty()) {
        throw std::logic_error("Empty collection");
      }
      IoGuard guard(out);
      out << std::fixed << std::setprecision(1) << sumArea(polygons) / static_cast< double >(polygons.size());
    }

    void areaNum(const std::string& arg, std::ostream& out, const std::vector< Polygon >& polygons)
    {
      const size_t n = std::stoull(arg);
      if (n < 3) {
        throw std::logic_error("Invalid N");
      }
      std::vector< Polygon > filtered;
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered),
        std::bind(hasVertices, std::placeholders::_1, n));
      IoGuard guard(out);
      out << std::fixed << std::setprecision(1) << sumArea(filtered);
    }

    void maxArea(std::ostream& out, const std::vector< Polygon >& polygons)
    {
      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);
      IoGuard guard(out);
      out << std::fixed << std::setprecision(1) << *std::max_element(areas.begin(), areas.end());
    }

    void maxVertexes(std::ostream& out, const std::vector< Polygon >& polygons)
    {
      std::vector< size_t > counts;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(counts), getVerticesCount);
      out << *std::max_element(counts.begin(), counts.end());
    }

    void minArea(std::ostream& out, const std::vector< Polygon >& polygons)
    {
      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);
      IoGuard guard(out);
      out << std::fixed << std::setprecision(1) << *std::min_element(areas.begin(), areas.end());
    }

    void minVertexes(std::ostream& out, const std::vector< Polygon >& polygons)
    {
      std::vector< size_t > counts;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(counts), getVerticesCount);
      out << *std::min_element(counts.begin(), counts.end());
    }

    void countEven(std::ostream& out, const std::vector< Polygon >& polygons)
    {
      out << std::count_if(polygons.begin(), polygons.end(), isEven);
    }

    void countOdd(std::ostream& out, const std::vector< Polygon >& polygons)
    {
      out << std::count_if(polygons.begin(), polygons.end(), isOdd);
    }

    void countNum(const std::string& arg, std::ostream& out, const std::vector< Polygon >& polygons)
    {
      const size_t n = std::stoull(arg);
      if (n < 3) {
        throw std::logic_error("Invalid N");
      }
      out << std::count_if(polygons.begin(), polygons.end(), std::bind(hasVertices, std::placeholders::_1, n));
    }
  }
}

void sogdanov::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string arg;
  in >> arg;
  checkTrailingGarbage(in);

  std::map< std::string, std::function< void(std::ostream&, const std::vector< Polygon >&) > > subcmds;
  subcmds["EVEN"] = areaEven;
  subcmds["ODD"] = areaOdd;
  subcmds["MEAN"] = areaMean;

  auto it = subcmds.find(arg);
  if (it != subcmds.end()) {
    it->second(out, polygons);
  } else {
    areaNum(arg, out, polygons);
  }
}

void sogdanov::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string arg;
  in >> arg;
  checkTrailingGarbage(in);
  if (polygons.empty()) {
    throw std::logic_error("Empty collection");
  }

  std::map< std::string, std::function< void(std::ostream&, const std::vector< Polygon >&) > > subcmds;
  subcmds["AREA"] = maxArea;
  subcmds["VERTEXES"] = maxVertexes;

  auto it = subcmds.find(arg);
  if (it != subcmds.end()) {
    it->second(out, polygons);
  } else {
    throw std::logic_error("Invalid arg");
  }
}

void sogdanov::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string arg;
  in >> arg;
  checkTrailingGarbage(in);
  if (polygons.empty()) {
    throw std::logic_error("Empty collection");
  }

  std::map< std::string, std::function< void(std::ostream&, const std::vector< Polygon >&) > > subcmds;
  subcmds["AREA"] = minArea;
  subcmds["VERTEXES"] = minVertexes;

  auto it = subcmds.find(arg);
  if (it != subcmds.end()) {
    it->second(out, polygons);
  } else {
    throw std::logic_error("Invalid arg");
  }
}

void sogdanov::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string arg;
  in >> arg;
  checkTrailingGarbage(in);

  std::map< std::string, std::function< void(std::ostream&, const std::vector< Polygon >&) > > subcmds;
  subcmds["EVEN"] = countEven;
  subcmds["ODD"] = countOdd;

  auto it = subcmds.find(arg);
  if (it != subcmds.end()) {
    it->second(out, polygons);
  } else {
    countNum(arg, out, polygons);
  }
}

void sogdanov::perms(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon target;
  if (!(in >> target)) {
    throw std::logic_error("Invalid polygon");
  }
  checkTrailingGarbage(in);
  out << std::count_if(polygons.begin(), polygons.end(), std::bind(isPermutation, std::placeholders::_1, target));
}

void sogdanov::maxseq(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon target;
  if (!(in >> target)) {
    throw std::logic_error("Invalid polygon");
  }
  checkTrailingGarbage(in);
  out << countMaxSeq(polygons.begin(), polygons.end(), target, 0);
}
