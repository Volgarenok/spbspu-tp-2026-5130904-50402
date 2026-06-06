#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <set>
#include <map>

namespace chernikov {

  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  int cross_product(const Point &a, const Point &b)
  {
    return a.x * b.y - a.y * b.x;
  }

  int dot_product(const Point &a, const Point &b)
  {
    return a.x * b.x + a.y * b.y;
  }

  Point subtract(const Point &a, const Point &b)
  {
    return {a.x - b.x, a.y - b.y};
  }

  double area(const Polygon &poly)
  {
    if (poly.points.size() < 3)
      return 0.0;

    std::vector< int > products(poly.points.size());

    auto it = poly.points.begin();
    auto next_it = poly.points.begin();
    ++next_it;

    for (size_t i = 0; i < poly.points.size(); ++i)
    {
      const Point &p1 = *it;
      const Point &p2 = *next_it;
      products[i] = p1.x * p2.y - p1.y * p2.x;

      ++it;
      ++next_it;
      if (next_it == poly.points.end())
      {
        next_it = poly.points.begin();
      }
    }

    int sum = std::accumulate(products.begin(), products.end(), 0);
    return std::abs(sum) / 2.0;
  }

  double distance_sq(const Point &a, const Point &b)
  {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
  }

  bool is_right_angle(const Point &a, const Point &b, const Point &c)
  {
    Point ab = subtract(b, a);
    Point bc = subtract(c, b);
    return dot_product(ab, bc) == 0;
  }

  bool has_right_angle(const Polygon &poly)
  {
    if (poly.points.size() < 3)
      return false;

    auto it1 = poly.points.begin();
    auto it2 = poly.points.begin();
    ++it2;
    auto it3 = poly.points.begin();
    ++it3;
    ++it3;

    for (size_t i = 0; i < poly.points.size(); ++i)
    {
      if (is_right_angle(*it1, *it2, *it3))
      {
        return true;
      }
      ++it1;
      ++it2;
      ++it3;
      if (it1 == poly.points.end())
        it1 = poly.points.begin();
      if (it2 == poly.points.end())
        it2 = poly.points.begin();
      if (it3 == poly.points.end())
        it3 = poly.points.begin();
    }
    return false;
  }

  bool is_rectangle(const Polygon &poly)
  {
    return poly.points.size() == 4 && has_right_angle(poly)
           && (distance_sq(poly.points[0], poly.points[1]) == distance_sq(poly.points[2], poly.points[3]))
           && (distance_sq(poly.points[1], poly.points[2]) == distance_sq(poly.points[3], poly.points[0]))
           && is_right_angle(poly.points[0], poly.points[1], poly.points[2])
           && is_right_angle(poly.points[2], poly.points[3], poly.points[0]);
  }

  bool is_permutation_of(const Polygon &a, const Polygon &b)
  {
    if (a.points.size() != b.points.size())
      return false;

    std::multiset< Point > set_a(a.points.begin(), a.points.end());
    std::multiset< Point > set_b(b.points.begin(), b.points.end());

    return set_a == set_b;
  }

  std::string trim(const std::string &s)
  {
    size_t start = 0;
    while (start < s.length() && s[start] == ' ')
      ++start;
    size_t end = s.length();
    while (end > start && s[end - 1] == ' ')
      --end;
    return s.substr(start, end - start);
  }

  struct SplitResult
  {
    std::string parts[100];
    size_t count;
  };

  SplitResult split(const std::string &line)
  {
    SplitResult result;
    result.count = 0;

    std::string current;
    for (size_t i = 0; i < line.length(); ++i)
    {
      if (line[i] == ' ')
      {
        if (!current.empty() && result.count < 100)
        {
          result.parts[result.count++] = current;
          current.clear();
        }
      } else
      {
        current += line[i];
      }
    }
    if (!current.empty() && result.count < 100)
    {
      result.parts[result.count++] = current;
    }

    return result;
  }

  int string_to_int(const std::string &s)
  {
    int result = 0;
    int sign = 1;
    size_t i = 0;

    if (s.length() > 0 && s[0] == '-')
    {
      sign = -1;
      i = 1;
    }

    for (; i < s.length(); ++i)
    {
      result = result * 10 + (s[i] - '0');
    }

    return result * sign;
  }

  bool parse_point(const std::string &s, Point &p)
  {
    if (s.length() < 4 || s[0] != '(' || s[s.length() - 1] != ')')
    {
      return false;
    }

    size_t semicolon = s.find(';');
    if (semicolon == std::string::npos)
      return false;

    std::string x_str = s.substr(1, semicolon - 1);
    std::string y_str = s.substr(semicolon + 1, s.length() - semicolon - 2);

    for (char c : x_str)
    {
      if (c != '-' && c != '+' && !std::isdigit(c))
        return false;
    }
    for (char c : y_str)
    {
      if (c != '-' && c != '+' && !std::isdigit(c))
        return false;
    }

    p.x = string_to_int(x_str);
    p.y = string_to_int(y_str);
    return true;
  }

  Polygon parse_polygon_from_tokens(const SplitResult &tokens, size_t start)
  {
    Polygon poly;
    int num_vertices = string_to_int(tokens.parts[start]);

    for (int i = 0; i < num_vertices && (start + 1 + i) < tokens.count; ++i)
    {
      Point p;
      parse_point(tokens.parts[start + 1 + i], p);
      poly.points.push_back(p);
    }

    return poly;
  }
}

int main()
{
  return 0;
}
