#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

namespace pozdnyakov
{

  struct Point
  {
    int x, y;
  };

  bool operator==(const Point &lhs, const Point &rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  bool operator!=(const Point &lhs, const Point &rhs)
  {
    return !(lhs == rhs);
  }

  struct Polygon
  {
    std::vector< Point > points;
  };

  bool operator==(const Polygon &lhs, const Polygon &rhs)
  {
    return lhs.points == rhs.points;
  }

  struct DelimiterIO
  {
    char expected;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.expected)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, Point &dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    Point temp{0, 0};
    in >> DelimiterIO{'('} >> temp.x >> DelimiterIO{';'} >> temp.y >> DelimiterIO{')'};
    if (in) {
      dest = temp;
    }
    return in;
  }

  struct PointReader
  {
    std::istream &in;
    Point operator()() const
    {
      Point p{0, 0};
      in >> p;
      return p;
    }
  };

  std::istream &operator>>(std::istream &in, Polygon &dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    size_t size = 0;
    in >> size;
    if (!in) {
      return in;
    }

    Polygon temp;
    std::generate_n(std::back_inserter(temp.points), size, PointReader{in});
    if (in) {
      dest = temp;
    }
    return in;
  }

  struct ShoelaceTerm
  {
    double operator()(const Point &p1, const Point &p2) const
    {
      return static_cast< double >(p1.x) * p2.y - static_cast< double >(p1.y) * p2.x;
    }
  };

  inline double getArea(const Polygon &poly)
  {
    if (poly.points.size() < 3) {
      return 0.0;
    }
    double sum = std::inner_product(poly.points.begin(), poly.points.end() - 1, poly.points.begin() + 1, 0.0,
                                    std::plus< double >(), ShoelaceTerm{});
    sum += ShoelaceTerm{}(poly.points.back(), poly.points.front());
    return std::abs(sum) / 2.0;
  }

  struct Segment
  {
    Point a, b;
  };

  struct MakeSegment
  {
    Segment operator()(const Point &a, const Point &b) const
    {
      return Segment{a, b};
    }
  };

  inline std::vector< Segment > getSegments(const Polygon &p)
  {
    std::vector< Segment > segs;
    if (p.points.size() < 2) {
      return segs;
    }
    std::transform(p.points.begin(), p.points.end() - 1, p.points.begin() + 1, std::back_inserter(segs), MakeSegment{});
    segs.push_back(MakeSegment{}(p.points.back(), p.points.front()));
    return segs;
  }

  inline int getOrientation(const Point &p, const Point &q, const Point &r)
  {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) {
      return 0;
    }
    return (val > 0) ? 1 : 2;
  }

  inline bool checkOnSegment(const Point &p, const Point &q, const Point &r)
  {
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y)
           && q.y >= std::min(p.y, r.y);
  }

  struct CheckSegmentIntersect
  {
    Segment s1;
    bool operator()(const Segment &s2) const
    {
      int o1 = getOrientation(s1.a, s1.b, s2.a);
      int o2 = getOrientation(s1.a, s1.b, s2.b);
      int o3 = getOrientation(s2.a, s2.b, s1.a);
      int o4 = getOrientation(s2.a, s2.b, s1.b);

      if (o1 != o2 && o3 != o4) {
        return true;
      }
      if (o1 == 0 && checkOnSegment(s1.a, s2.a, s1.b)) {
        return true;
      }
      if (o2 == 0 && checkOnSegment(s1.a, s2.b, s1.b)) {
        return true;
      }
      if (o3 == 0 && checkOnSegment(s2.a, s1.a, s2.b)) {
        return true;
      }
      if (o4 == 0 && checkOnSegment(s2.a, s1.b, s2.b)) {
        return true;
      }
      return false;
    }
  };

  struct CheckAnySegmentIntersect
  {
    const std::vector< Segment > &segs2;
    bool operator()(const Segment &s1) const
    {
      return std::any_of(segs2.begin(), segs2.end(), CheckSegmentIntersect{s1});
    }
  };

  struct RayIntersectCheck
  {
    Point p;
    bool operator()(const Segment &seg) const
    {
      if ((seg.a.y > p.y) != (seg.b.y > p.y)) {
        double intersectX =
            seg.a.x
            + static_cast< double >(p.y - seg.a.y) * (seg.b.x - seg.a.x) / static_cast< double >(seg.b.y - seg.a.y);
        if (static_cast< double >(p.x) < intersectX) {
          return true;
        }
      }
      return false;
    }
  };

  struct CheckPointInsidePolygon
  {
    const std::vector< Segment > &polySegs;
    bool operator()(const Point &pt) const
    {
      size_t intersections = std::count_if(polySegs.begin(), polySegs.end(), RayIntersectCheck{pt});
      return (intersections % 2) == 1;
    }
  };

  struct CheckPermutation
  {
    const Polygon &target;
    bool operator()(const Polygon &p) const
    {
      if (p.points.size() != target.points.size()) {
        return false;
      }
      return std::is_permutation(p.points.begin(), p.points.end(), target.points.begin(), target.points.end());
    }
  };

  struct CheckPolygonsIntersect
  {
    const Polygon &p1;
    bool operator()(const Polygon &p2) const
    {
      auto segs1 = getSegments(p1);
      auto segs2 = getSegments(p2);
      if (std::any_of(segs1.begin(), segs1.end(), CheckAnySegmentIntersect{segs2})) {
        return true;
      }
      if (std::any_of(p1.points.begin(), p1.points.end(), CheckPointInsidePolygon{segs2})) {
        return true;
      }
      if (std::any_of(p2.points.begin(), p2.points.end(), CheckPointInsidePolygon{segs1})) {
        return true;
      }
      return false;
    }
  };

  struct AreaIfEven
  {
    double operator()(const Polygon &p) const
    {
      return (p.points.size() % 2 == 0) ? getArea(p) : 0.0;
    }
  };

  struct AreaIfOdd
  {
    double operator()(const Polygon &p) const
    {
      return (p.points.size() % 2 != 0) ? getArea(p) : 0.0;
    }
  };

  struct AreaIfNum
  {
    size_t num;
    double operator()(const Polygon &p) const
    {
      return (p.points.size() == num) ? getArea(p) : 0.0;
    }
  };

  struct GetArea
  {
    double operator()(const Polygon &p) const
    {
      return getArea(p);
    }
  };

  struct IsEvenVertices
  {
    bool operator()(const Polygon &p) const
    {
      return p.points.size() % 2 == 0;
    }
  };

  struct IsOddVertices
  {
    bool operator()(const Polygon &p) const
    {
      return p.points.size() % 2 != 0;
    }
  };

  struct IsNumVertices
  {
    size_t num;
    bool operator()(const Polygon &p) const
    {
      return p.points.size() == num;
    }
  };

  struct CompareArea
  {
    bool operator()(const Polygon &p1, const Polygon &p2) const
    {
      return getArea(p1) < getArea(p2);
    }
  };

  struct CompareVertices
  {
    bool operator()(const Polygon &p1, const Polygon &p2) const
    {
      return p1.points.size() < p2.points.size();
    }
  };

  struct IsDigit
  {
    bool operator()(char c) const
    {
      return std::isdigit(static_cast< unsigned char >(c));
    }
  };

  inline bool isNumericString(const std::string &str)
  {
    if (str.empty()) {
      return false;
    }
    return std::all_of(str.begin(), str.end(), IsDigit{});
  }

  inline double calculateAreaEven(const std::vector< Polygon > &polygons)
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), AreaIfEven{});
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  inline double calculateAreaOdd(const std::vector< Polygon > &polygons)
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), AreaIfOdd{});
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  inline double calculateAreaMean(const std::vector< Polygon > &polygons)
  {
    if (polygons.empty()) {
      return 0.0;
    }
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), GetArea{});
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    return sum / polygons.size();
  }

  inline double calculateAreaNum(const std::vector< Polygon > &polygons, size_t num)
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), AreaIfNum{num});
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  inline double getMinArea(const std::vector< Polygon > &polygons)
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), CompareArea{});
    if (it != polygons.end()) {
      return getArea(*it);
    }
    return 0.0;
  }

  inline size_t getMinVertexes(const std::vector< Polygon > &polygons)
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), CompareVertices{});
    if (it != polygons.end()) {
      return it->points.size();
    }
    return 0;
  }

  inline double getMaxArea(const std::vector< Polygon > &polygons)
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), CompareArea{});
    if (it != polygons.end()) {
      return getArea(*it);
    }
    return 0.0;
  }

  inline size_t getMaxVertexes(const std::vector< Polygon > &polygons)
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), CompareVertices{});
    if (it != polygons.end()) {
      return it->points.size();
    }
    return 0;
  }

  inline size_t countEvenVertices(const std::vector< Polygon > &polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), IsEvenVertices{});
  }

  inline size_t countOddVertices(const std::vector< Polygon > &polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), IsOddVertices{});
  }

  inline size_t countNumVertices(const std::vector< Polygon > &polygons, size_t num)
  {
    return std::count_if(polygons.begin(), polygons.end(), IsNumVertices{num});
  }

  inline size_t countPermutations(const std::vector< Polygon > &polygons, const Polygon &target)
  {
    return std::count_if(polygons.begin(), polygons.end(), CheckPermutation{target});
  }

  inline size_t countIntersections(const std::vector< Polygon > &polygons, const Polygon &target)
  {
    return std::count_if(polygons.begin(), polygons.end(), CheckPolygonsIntersect{target});
  }

  inline void processArea(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string arg;
    in >> arg;
    if (arg == "EVEN") {
      out << calculateAreaEven(polygons) << '\n';
    } else if (arg == "ODD") {
      out << calculateAreaOdd(polygons) << '\n';
    } else if (arg == "MEAN") {
      out << calculateAreaMean(polygons) << '\n';
    } else if (isNumericString(arg)) {
      out << calculateAreaNum(polygons, std::stoull(arg)) << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  inline void processMin(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string arg;
    in >> arg;
    if (arg == "AREA") {
      out << getMinArea(polygons) << '\n';
    } else if (arg == "VERTEXES") {
      out << getMinVertexes(polygons) << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  inline void processMax(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string arg;
    in >> arg;
    if (arg == "AREA") {
      out << getMaxArea(polygons) << '\n';
    } else if (arg == "VERTEXES") {
      out << getMaxVertexes(polygons) << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  inline void processCount(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string arg;
    in >> arg;
    if (arg == "EVEN") {
      out << countEvenVertices(polygons) << '\n';
    } else if (arg == "ODD") {
      out << countOddVertices(polygons) << '\n';
    } else if (isNumericString(arg)) {
      out << countNumVertices(polygons, std::stoull(arg)) << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  inline void processPerms(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    Polygon target;
    if (in >> target) {
      out << countPermutations(polygons, target) << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  inline void processIntersections(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    Polygon target;
    if (in >> target) {
      out << countIntersections(polygons, target) << '\n';
    } else {
      throw std::invalid_argument("invalid");
    }
  }

  inline void readPolygons(std::istream &in, std::vector< Polygon > &polygons)
  {
    std::copy(std::istream_iterator< Polygon >(in), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
    if (in.fail() && !in.eof()) {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      readPolygons(in, polygons);
    }
  }

  struct CommandExecutor
  {
    const std::map< std::string, std::function< void() > > &commands;

    bool operator()(const std::string &cmd) const
    {
      try {
        commands.at(cmd)();
      } catch (const std::exception &) {
        if (std::cin.fail()) {
          std::cin.clear();
        }
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        std::cout << "Invalid command\n";
      }
      return false;
    }
  };

}

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

  std::vector< pozdnyakov::Polygon > polygons;
  pozdnyakov::readPolygons(file, polygons);

  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(pozdnyakov::processArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(pozdnyakov::processMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(pozdnyakov::processMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(pozdnyakov::processCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["PERMS"] = std::bind(pozdnyakov::processPerms, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["INTERSECTIONS"] =
      std::bind(pozdnyakov::processIntersections, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::cout << std::fixed << std::setprecision(1);

  std::any_of(std::istream_iterator< std::string >(std::cin), std::istream_iterator< std::string >(),
              pozdnyakov::CommandExecutor{commands});

  return 0;
}
