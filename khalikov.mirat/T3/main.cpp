#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <functional>
#include <numeric>

namespace khalikov
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  struct DelimiterIO
  {
    char exp;
  };

  bool operator==(const Point &lhs, const Point &rhs);
  bool operator==(const Polygon &lhs, const Polygon &rhs);

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, Point &dest);
  std::istream &operator>>(std::istream &in, Polygon &dest);
  std::ostream &operator<<(std::ostream &out, const Point &src);
  std::ostream &operator<<(std::ostream &out, const Polygon &src);

  size_t countMaxSeq(std::vector< Polygon >::const_iterator first, std::vector< Polygon >::const_iterator last,
                     const Polygon &example, size_t currMax);

  void getFrame(std::vector< Polygon >::const_iterator first, std::vector< Polygon >::const_iterator last, int &minX,
                int &minY, int &maxX, int &maxY);

  bool compareVertexes(const Polygon &lhs, const Polygon &rhs);
  bool compareArea(const Polygon &lhs, const Polygon &rhs);
  bool compX(const Point &lhs, const Point &rhs);
  bool compY(const Point &lhs, const Point &rhs);
  bool isPointInFrame(int minX, int minY, int maxX, int maxY, const Point &pt);
  bool isEven(const Polygon &poly);
  bool isOdd(const Polygon &poly);
  bool isNum(size_t num, const Polygon &poly);
  double crossTerm(const std::vector< Point > &pts, size_t i, size_t n);
  double calcArea(const Polygon &poly);
  double sumArea(const std::function< bool(const Polygon &) > &pred, double acc, const Polygon &poly);
  double sumExactArea(size_t n, double acc, const Polygon &poly);
  double sumAllArea(double acc, const Polygon &poly);

  void show(std::istream &, std::ostream &out, const std::vector< Polygon > &polygons);
  void maxSeq(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void inFrame(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
  void min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons);
}

int main(int argc, char **argv)
{
  using iit_t = std::istream_iterator< khalikov::Polygon >;
  if (argc != 2) {
    std::cerr << "Input error\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cannot open file\n";
    return 1;
  }
  std::vector< khalikov::Polygon > polygons;
  while (!file.eof()) {
    std::copy(iit_t(file), iit_t(), std::back_inserter(polygons));
    if (!file) {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::unordered_map< std::string, std::function< void() > > cmds;
  cmds["SHOW"] = std::bind(khalikov::show, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAXSEQ"] = std::bind(khalikov::maxSeq, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["INFRAME"] = std::bind(khalikov::inFrame, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COUNT"] = std::bind(khalikov::count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["AREA"] = std::bind(khalikov::area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAX"] = std::bind(khalikov::max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MIN"] = std::bind(khalikov::min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)();
    } catch (const std::exception &e) {
      if (std::cin.fail()) {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

bool khalikov::compareArea(const Polygon &lhs, const Polygon &rhs)
{
  return calcArea(lhs) < calcArea(rhs);
}

bool khalikov::compareVertexes(const Polygon &lhs, const Polygon &rhs)
{
  return lhs.points.size() < rhs.points.size();
}

void khalikov::min(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string arg;
  if (!(in >> arg) || polygons.empty()) {
    throw std::invalid_argument("Input error");
  }
  if (in.peek() != EOF && in.peek() != '\n') {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  if (arg == "AREA") {
    auto it = std::min_element(polygons.begin(), polygons.end(), compareArea);
    out << std::fixed << std::setprecision(1) << calcArea(*it) << '\n';
  } else if (arg == "VERTEXES") {
    auto it = std::min_element(polygons.begin(), polygons.end(), compareVertexes);
    out << it->points.size() << '\n';
  } else {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
}

void khalikov::max(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string arg;
  if (!(in >> arg) || polygons.empty()) {
    throw std::invalid_argument("Input error");
  }
  if (in.peek() != EOF && in.peek() != '\n') {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  if (arg == "AREA") {
    auto it = std::max_element(polygons.begin(), polygons.end(), compareArea);
    out << std::fixed << std::setprecision(1) << calcArea(*it) << '\n';
  } else if (arg == "VERTEXES") {
    auto it = std::max_element(polygons.begin(), polygons.end(), compareVertexes);
    out << it->points.size() << '\n';
  } else {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
}

double khalikov::crossTerm(const std::vector< Point > &pts, size_t i, size_t n)
{
  size_t j = (i + 1) % n;
  return pts[i].x * pts[j].y - pts[j].x * pts[i].y;
}

double khalikov::calcArea(const Polygon &poly)
{
  size_t n = poly.points.size();
  if (n < 3) {
    return 0.0;
  }
  std::vector< size_t > idxs(n);
  std::iota(idxs.begin(), idxs.end(), 0);
  std::vector< double > areas(n);
  using namespace std::placeholders;
  auto g = std::bind(crossTerm, std::cref(poly.points), _1, n);
  std::transform(idxs.begin(), idxs.end(), areas.begin(), g);
  double sum = std::accumulate(areas.begin(), areas.end(), 0.0, std::plus< double >());
  return std::abs(sum) / 2.0;
}

double khalikov::sumArea(const std::function< bool(const Polygon &) > &pred, double acc, const Polygon &poly)
{
  if (pred(poly)) {
    return acc + calcArea(poly);
  }
  return acc;
}

bool khalikov::isNum(size_t num, const Polygon &poly)
{
  return (poly.points.size() == num);
}

bool khalikov::isOdd(const Polygon &poly)
{
  return ((poly.points.size() % 2) != 0);
}

bool khalikov::isEven(const Polygon &poly)
{
  return ((poly.points.size() % 2) == 0);
}

bool khalikov::compX(const Point &lhs, const Point &rhs)
{
  return lhs.x < rhs.x;
}

bool khalikov::compY(const Point &lhs, const Point &rhs)
{
  return lhs.y < rhs.y;
}

bool khalikov::isPointInFrame(int minX, int minY, int maxX, int maxY, const Point &pt)
{
  return (pt.x >= minX && pt.x <= maxX && pt.y >= minY && pt.y <= maxY);
}

double khalikov::sumAllArea(double acc, const khalikov::Polygon &poly)
{
  return acc + calcArea(poly);
}

double khalikov::sumExactArea(size_t n, double acc, const khalikov::Polygon &poly)
{
  if (poly.points.size() == n) {
    return acc + calcArea(poly);
  }
  return acc;
}

using it_t = std::vector< khalikov::Polygon >::const_iterator;
size_t khalikov::countMaxSeq(it_t first, it_t last, const khalikov::Polygon &example, size_t currMax)
{
  const auto start = std::find(first, last, example);
  if (start == last) {
    return currMax;
  }
  using namespace std::placeholders;
  const auto isEqual = std::bind(std::equal_to< khalikov::Polygon >(), _1, example);
  const auto end = std::find_if_not(start, last, isEqual);
  size_t res = static_cast< size_t >(std::distance(start, end));
  return countMaxSeq(end, last, example, std::max(currMax, res));
}

void khalikov::getFrame(it_t first, it_t last, int &minX, int &minY, int &maxX, int &maxY)
{
  if (first == last) {
    return;
  }
  auto x_bounds = std::minmax_element(first->points.begin(), first->points.end(), compX);
  auto y_bounds = std::minmax_element(first->points.begin(), first->points.end(), compY);
  minX = std::min(minX, x_bounds.first->x);
  maxX = std::max(maxX, x_bounds.second->x);
  minY = std::min(minY, y_bounds.first->y);
  maxY = std::max(maxY, y_bounds.second->y);
  getFrame(std::next(first), last, minX, minY, maxX, maxY);
}

void khalikov::area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string arg;
  if (!(in >> arg)) {
    throw std::invalid_argument("Input error");
  }
  if (in.peek() != EOF && in.peek() != '\n') {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  double res = 0.0;
  out << std::fixed << std::setprecision(1);
  using namespace std::placeholders;
  if (arg == "EVEN") {
    auto f = std::bind(sumArea, isEven, _1, _2);
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, f);
    out << res << '\n';
  } else if (arg == "ODD") {
    auto f = std::bind(sumArea, isOdd, _1, _2);
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, f);
    out << res << '\n';
  } else if (arg == "MEAN") {
    if (polygons.empty()) {
      throw std::invalid_argument("Input error");
    }
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, sumAllArea);
    out << (res / polygons.size()) << '\n';
  } else {
    try {
      size_t n = std::stoull(arg);
      if (n < 3) {
        throw std::invalid_argument("Input error");
      }
      auto f = std::bind(sumExactArea, n, _1, _2);
      res = std::accumulate(polygons.begin(), polygons.end(), 0.0, f);
      out << res << '\n';
    } catch (...) {
      throw std::invalid_argument("Input error");
    }
  }
}

void khalikov::count(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string arg;
  if (!(in >> arg)) {
    throw std::invalid_argument("Input error");
  }
  size_t res = 0;
  using namespace std::placeholders;
  if (arg == "EVEN") {
    res = std::count_if(polygons.begin(), polygons.end(), isEven);
  } else if (arg == "ODD") {
    res = std::count_if(polygons.begin(), polygons.end(), isOdd);
  } else {
    try {
      size_t num = std::stoull(arg);
      if (num < 3) {
        throw std::invalid_argument("Input error");
      }
      res = std::count_if(polygons.begin(), polygons.end(), std::bind(isNum, num, _1));
    } catch (...) {
      throw std::invalid_argument("Input error");
    }
  }
  out << res << '\n';
}

void khalikov::inFrame(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  khalikov::Polygon example;
  if (!(in >> example)) {
    throw std::invalid_argument("Input error");
  }
  if (in.peek() != EOF && in.peek() != '\n') {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  int minX = std::numeric_limits< int >::max();
  int minY = std::numeric_limits< int >::max();
  int maxX = std::numeric_limits< int >::min();
  int maxY = std::numeric_limits< int >::min();
  getFrame(polygons.begin(), polygons.end(), minX, minY, maxX, maxY);
  using namespace std::placeholders;
  const auto checkPoint = std::bind(isPointInFrame, minX, minY, maxX, maxY, _1);
  if (std::all_of(example.points.begin(), example.points.end(), checkPoint)) {
    out << "<TRUE>\n";
  } else {
    out << "<FALSE>\n";
  }
}

void khalikov::maxSeq(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  khalikov::Polygon example;
  if (!(in >> example)) {
    throw std::invalid_argument("Input error");
  }
  if (in.peek() != EOF && in.peek() != '\n') {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  size_t res = khalikov::countMaxSeq(polygons.begin(), polygons.end(), example, 0);
  out << res << '\n';
}

bool khalikov::operator==(const Point &lhs, const Point &rhs)
{
  return (lhs.x == rhs.x && lhs.y == rhs.y);
}

bool khalikov::operator==(const Polygon &lhs, const Polygon &rhs)
{
  return lhs.points == rhs.points;
}

void khalikov::show(std::istream &, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::copy(polygons.begin(), polygons.end(), std::ostream_iterator< Polygon >(out, "\n"));
}

std::ostream &khalikov::operator<<(std::ostream &out, const Point &src)
{
  out << '(' << src.x << ';' << src.y << ')';
  return out;
}

std::ostream &khalikov::operator<<(std::ostream &out, const Polygon &src)
{
  out << src.points.size();
  if (!src.points.empty()) {
    out << ' ';
    std::copy(src.points.begin(), src.points.end() - 1, std::ostream_iterator< Point >(out, " "));
    out << src.points.back();
  }
  return out;
}

std::istream &khalikov::operator>>(std::istream &in, Polygon &dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  size_t count = 0;
  if (!(in >> count) || count < 3) {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp;
  temp.reserve(count);
  using iit_t = std::istream_iterator< khalikov::Point >;
  std::copy_n(iit_t(in), count, std::back_inserter(temp));
  if (in) {
    dest.points = std::move(temp);
  }
  return in;
}

std::istream &khalikov::operator>>(std::istream &in, Point &dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  Point pt{0, 0};
  in >> DelimiterIO{'('} >> pt.x >> DelimiterIO{';'} >> pt.y >> DelimiterIO{')'};
  if (in) {
    dest = pt;
  }
  return in;
}

std::istream &khalikov::operator>>(std::istream &in, DelimiterIO &&dest)
{
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}
