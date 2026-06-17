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

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, Point &dest);
  std::istream &operator>>(std::istream &in, Polygon &dest);
  std::ostream &operator<<(std::ostream &out, const Point &src);
  std::ostream &operator<<(std::ostream &out, const Polygon &src);

  void show(std::istream &, std::ostream &out, const std::vector< Polygon > &polygons);

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
  cmds["show"] = std::bind(khalikov::show, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)();
    } catch (const std::exception &e) {
      if (std::cin.fail()) {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "INVALID COMMAND\n";
    }
  }
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
