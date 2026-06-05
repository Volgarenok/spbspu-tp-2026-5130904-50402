#include "Commands.hpp"
#include "IoUtils.hpp"
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <cmath>

namespace sogdanov {

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

  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string arg;
    in >> arg;
    std::vector< Polygon > filtered;
    IoGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (arg == "EVEN") {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isEven);
      out << sumArea(filtered) << '\n';
    } else if (arg == "ODD") {
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isOdd);
      out << sumArea(filtered) << '\n';
    } else if (arg == "MEAN") {
      if (polygons.empty()) {
        throw std::logic_error("Empty collection");
      }
      out << sumArea(polygons) / static_cast< double >(polygons.size()) << '\n';
    } else {
      const size_t n = std::stoull(arg);
      if (n < 3) {
        throw std::logic_error("Invalid N");
      }
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered),
        std::bind(hasVertices, std::placeholders::_1, n));
      out << sumArea(filtered) << '\n';
    }
  }

  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    if (polygons.empty()) {
      throw std::logic_error("Empty collection");
    }
    std::string arg;
    in >> arg;
    if (arg == "AREA") {
      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);
      IoGuard guard(out);
      out << std::fixed << std::setprecision(1) << *std::max_element(areas.begin(), areas.end()) << '\n';
    } else if (arg == "VERTEXES") {
      std::vector< size_t > counts;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(counts), getVerticesCount);
      out << *std::max_element(counts.begin(), counts.end()) << '\n';
    } else {
      throw std::logic_error("Invalid arg");
    }
  }

  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    if (polygons.empty()) {
      throw std::logic_error("Empty collection");
    }
    std::string arg;
    in >> arg;
    if (arg == "AREA") {
      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);
      IoGuard guard(out);
      out << std::fixed << std::setprecision(1) << *std::min_element(areas.begin(), areas.end()) << '\n';
    } else if (arg == "VERTEXES") {
      std::vector< size_t > counts;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(counts), getVerticesCount);
      out << *std::min_element(counts.begin(), counts.end()) << '\n';
    } else {
      throw std::logic_error("Invalid arg");
    }
  }

  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string arg;
    in >> arg;
    if (arg == "EVEN") {
      out << std::count_if(polygons.begin(), polygons.end(), isEven) << '\n';
    } else if (arg == "ODD") {
      out << std::count_if(polygons.begin(), polygons.end(), isOdd) << '\n';
    } else {
      const size_t n = std::stoull(arg);
      if (n < 3) {
        throw std::logic_error("Invalid N");
      }
      out << std::count_if(polygons.begin(), polygons.end(),
        std::bind(hasVertices, std::placeholders::_1, n)) << '\n';
    }
  }

  void perms(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    Polygon target;
    if (!(in >> target)) {
      throw std::logic_error("Invalid polygon");
    }
    out << std::count_if(polygons.begin(), polygons.end(),
      std::bind(isPermutation, std::placeholders::_1, target)) << '\n';
  }

  void maxseq(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    Polygon target;
    if (!(in >> target)) {
      throw std::logic_error("Invalid polygon");
    }
    out << countMaxSeq(polygons.begin(), polygons.end(), target, 0) << '\n';
  }

}
