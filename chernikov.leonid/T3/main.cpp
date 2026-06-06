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

  bool operator==(const Point &a, const Point &b)
  {
    return a.x == b.x && a.y == b.y;
  }

  bool operator<(const Point &a, const Point &b)
  {
    if (a.x != b.x)
      return a.x < b.x;
    return a.y < b.y;
  }

  bool operator==(const Polygon &a, const Polygon &b)
  {
    if (a.points.size() != b.points.size())
      return false;

    for (size_t shift = 0; shift < a.points.size(); ++shift)
    {
      bool match = true;
      for (size_t i = 0; i < a.points.size(); ++i)
      {
        size_t idx = (i + shift) % a.points.size();
        if (!(a.points[idx] == b.points[i]))
        {
          match = false;
          break;
        }
      }
      if (match)
        return true;
    }
    return false;
  }

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

  std::istream &operator>>(std::istream &in, Polygon &poly)
  {
    std::string line;
    if (!std::getline(in, line))
    {
      return in;
    }

    line = trim(line);
    if (line.empty())
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    SplitResult tokens = split(line);
    if (tokens.count < 4)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    int num_vertices = string_to_int(tokens.parts[0]);
    if (num_vertices < 3 || static_cast< size_t >(num_vertices) != tokens.count - 1)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    poly.points.clear();
    for (int i = 0; i < num_vertices; ++i)
    {
      Point p;
      if (!parse_point(tokens.parts[i + 1], p))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      poly.points.push_back(p);
    }

    return in;
  }

  std::ostream &operator<<(std::ostream &out, const Point &p)
  {
    out << "(" << p.x << ";" << p.y << ")";
    return out;
  }

  std::ostream &operator<<(std::ostream &out, const Polygon &poly)
  {
    out << poly.points.size();
    for (size_t i = 0; i < poly.points.size(); ++i)
    {
      out << " " << poly.points[i];
    }
    return out;
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
  void cmd_area(const std::vector< Polygon > &polygons, const SplitResult &tokens)
  {
    if (tokens.count < 2)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    std::string param = tokens.parts[1];

    if (param == "EVEN")
    {
      auto count = std::count_if(polygons.begin(), polygons.end(), [](const Polygon &p) {
        return p.points.size() % 2 == 0;
      });
      if (count == 0)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), [](const Polygon &p) {
        return p.points.size() % 2 == 0 ? area(p) : 0.0;
      });

      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      std::cout << std::fixed;
      std::cout.precision(1);
      std::cout << total << "\n";
    } else if (param == "ODD")
    {
      auto count = std::count_if(polygons.begin(), polygons.end(), [](const Polygon &p) {
        return p.points.size() % 2 != 0;
      });
      if (count == 0)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), [](const Polygon &p) {
        return p.points.size() % 2 != 0 ? area(p) : 0.0;
      });

      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      std::cout << std::fixed;
      std::cout.precision(1);
      std::cout << total << "\n";
    } else if (param == "MEAN")
    {
      if (polygons.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), [](const Polygon &p) {
        return area(p);
      });

      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      double mean = total / polygons.size();
      std::cout << std::fixed;
      std::cout.precision(1);
      std::cout << mean << "\n";
    } else
    {
      int num = string_to_int(param);
      if (num < 3)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), [num](const Polygon &p) {
        return static_cast< int >(p.points.size()) == num ? area(p) : 0.0;
      });

      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      std::cout << std::fixed;
      std::cout.precision(1);
      std::cout << total << "\n";
    }
  }

  void cmd_max(const std::vector< Polygon > &polygons, const SplitResult &tokens)
  {
    if (tokens.count < 2 || polygons.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    std::string param = tokens.parts[1];

    if (param == "AREA")
    {
      auto max_it = std::max_element(polygons.begin(), polygons.end(), [](const Polygon &a, const Polygon &b) {
        return area(a) < area(b);
      });

      std::cout << std::fixed;
      std::cout.precision(1);
      std::cout << area(*max_it) << "\n";
    } else if (param == "VERTEXES")
    {
      auto max_it = std::max_element(polygons.begin(), polygons.end(), [](const Polygon &a, const Polygon &b) {
        return a.points.size() < b.points.size();
      });

      std::cout << (*max_it).points.size() << "\n";
    } else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  void cmd_min(const std::vector< Polygon > &polygons, const SplitResult &tokens)
  {
    if (tokens.count < 2 || polygons.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    std::string param = tokens.parts[1];

    if (param == "AREA")
    {
      auto min_it = std::min_element(polygons.begin(), polygons.end(), [](const Polygon &a, const Polygon &b) {
        return area(a) < area(b);
      });

      std::cout << std::fixed;
      std::cout.precision(1);
      std::cout << area(*min_it) << "\n";
    } else if (param == "VERTEXES")
    {
      auto min_it = std::min_element(polygons.begin(), polygons.end(), [](const Polygon &a, const Polygon &b) {
        return a.points.size() < b.points.size();
      });

      std::cout << (*min_it).points.size() << "\n";
    } else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  void cmd_count(const std::vector< Polygon > &polygons, const SplitResult &tokens)
  {
    if (tokens.count < 2)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    std::string param = tokens.parts[1];

    if (param == "EVEN")
    {
      auto count = std::count_if(polygons.begin(), polygons.end(), [](const Polygon &p) {
        return p.points.size() % 2 == 0;
      });
      std::cout << count << "\n";
    } else if (param == "ODD")
    {
      auto count = std::count_if(polygons.begin(), polygons.end(), [](const Polygon &p) {
        return p.points.size() % 2 != 0;
      });
      std::cout << count << "\n";
    } else
    {
      int num = string_to_int(param);
      if (num < 3)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      auto count = std::count_if(polygons.begin(), polygons.end(), [num](const Polygon &p) {
        return static_cast< int >(p.points.size()) == num;
      });
      std::cout << count << "\n";
    }
  }

  void cmd_perms(const std::vector< Polygon > &polygons, const SplitResult &tokens)
  {
    if (tokens.count < 2)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    Polygon target = parse_polygon_from_tokens(tokens, 1);
    if (target.points.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    auto count = std::count_if(polygons.begin(), polygons.end(), [&target](const Polygon &p) {
      return is_permutation_of(p, target);
    });

    std::cout << count << "\n";
  }

  void cmd_maxseq(const std::vector< Polygon > &polygons, const SplitResult &tokens)
  {
    if (tokens.count < 2)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    Polygon target = parse_polygon_from_tokens(tokens, 1);
    if (target.points.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    size_t max_seq = 0;
    size_t current_seq = 0;

    for (size_t i = 0; i < polygons.size(); ++i)
    {
      if (polygons[i] == target)
      {
        ++current_seq;
        if (current_seq > max_seq)
        {
          max_seq = current_seq;
        }
      } else
      {
        current_seq = 0;
      }
    }

    std::cout << max_seq << "\n";
  }

  void cmd_rmecho(std::vector< Polygon > &polygons, const SplitResult &tokens)
  {
    if (tokens.count < 2)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    Polygon target = parse_polygon_from_tokens(tokens, 1);
    if (target.points.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    std::vector< Polygon > result;
    int removed = 0;

    for (size_t i = 0; i < polygons.size(); ++i)
    {
      if (polygons[i] == target)
      {
        if (result.empty() || !(result.back() == target))
        {
          result.push_back(polygons[i]);
        } else
        {
          ++removed;
        }
      } else
      {
        result.push_back(polygons[i]);
      }
    }

    polygons = std::move(result);
    std::cout << removed << "\n";
  }

  void cmd_rects(const std::vector< Polygon > &polygons)
  {
    auto count = std::count_if(polygons.begin(), polygons.end(), [](const Polygon &p) {
      return is_rectangle(p);
    });

    std::cout << count << "\n";
  }

  void cmd_rightshapes(const std::vector< Polygon > &polygons)
  {
    auto count = std::count_if(polygons.begin(), polygons.end(), [](const Polygon &p) {
      return has_right_angle(p);
    });

    std::cout << count << "\n";
  }

}

int main(int argc, char *argv[])
{
  return 0;
}