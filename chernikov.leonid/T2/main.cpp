#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <complex>
#include <utility>
#include <cmath>
#include <cctype>

namespace chernikov {

  struct DataStruct
  {
    std::complex< double > key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };

  double key2_to_double(const std::pair< long long, unsigned long long > &p)
  {
    return static_cast< double >(p.first) / static_cast< double >(p.second);
  }

  bool compare_data(const DataStruct &a, const DataStruct &b)
  {
    double abs_a = std::abs(a.key1);
    double abs_b = std::abs(b.key1);

    if (abs_a != abs_b)
    {
      return abs_a < abs_b;
    }

    double val_a = key2_to_double(a.key2);
    double val_b = key2_to_double(b.key2);

    if (val_a != val_b)
    {
      return val_a < val_b;
    }

    return a.key3.length() < b.key3.length();
  }

  std::string read_string(std::istream &in)
  {
    std::string result;
    char c;

    if (!(in >> c) || c != '"')
    {
      in.setstate(std::ios::failbit);
      return "";
    }

    while (in.get(c))
    {
      if (c == '"')
      {
        return result;
      }
      if (c == '\\')
      {
        if (in.get(c))
        {
          result += c;
        }
      } else
      {
        result += c;
      }
    }

    in.setstate(std::ios::failbit);
    return "";
  }
  bool parse_complex(const std::string &s, std::complex< double > &result)
  {
    if (s.length() < 4 || s[0] != '#' || s[1] != 'c' || s[2] != '(')
    {
      return false;
    }

    size_t pos = 3;
    while (pos < s.length() && s[pos] == ' ')
      ++pos;

    std::string real_str;
    while (pos < s.length() && s[pos] != ' ')
    {
      real_str += s[pos];
      ++pos;
    }

    while (pos < s.length() && s[pos] == ' ')
      ++pos;

    std::string imag_str;
    while (pos < s.length() && s[pos] != ')')
    {
      imag_str += s[pos];
      ++pos;
    }

    if (pos >= s.length() || s[pos] != ')')
    {
      return false;
    }

    char *end;
    double real = std::strtod(real_str.c_str(), &end);
    if (*end != '\0')
      return false;

    double imag = std::strtod(imag_str.c_str(), &end);
    if (*end != '\0')
      return false;

    result = std::complex< double >(real, imag);
    return true;
  }

  bool parse_rational(const std::string &s, std::pair< long long, unsigned long long > &result)
  {
    if (s.length() < 2 || s[0] != '(')
    {
      return false;
    }

    size_t pos = 1;

    if (pos + 1 >= s.length() || s[pos] != ':' || s[pos + 1] != 'N')
    {
      return false;
    }
    pos += 2;
    while (pos < s.length() && s[pos] == ' ')
      ++pos;

    std::string num_str;
    bool negative = false;
    if (pos < s.length() && s[pos] == '-')
    {
      negative = true;
      ++pos;
    }
    while (pos < s.length() && std::isdigit(s[pos]))
    {
      num_str += s[pos];
      ++pos;
    }

    while (pos < s.length() && s[pos] == ' ')
      ++pos;

    if (pos + 1 >= s.length() || s[pos] != ':' || s[pos + 1] != 'D')
    {
      return false;
    }
    pos += 2;
    while (pos < s.length() && s[pos] == ' ')
      ++pos;

    std::string den_str;
    while (pos < s.length() && std::isdigit(s[pos]))
    {
      den_str += s[pos];
      ++pos;
    }

    if (pos >= s.length() || s[pos] != ':' || pos + 1 >= s.length() || s[pos + 1] != ')')
    {
      return false;
    }

    char *end;
    long long num = std::strtoll(num_str.c_str(), &end, 10);
    if (*end != '\0')
      return false;
    if (negative)
      num = -num;

    unsigned long long den = std::strtoull(den_str.c_str(), &end, 10);
    if (*end != '\0')
      return false;
    if (den == 0)
      return false;

    result = std::make_pair(num, den);
    return true;
  }

  std::istream &operator>>(std::istream &in, DataStruct &data)
  {
    std::string line;
    if (!std::getline(in, line))
    {
      return in;
    }

    if (line.empty() || line[0] != '(' || line[line.length() - 1] != ':')
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    bool has_key1 = false;
    bool has_key2 = false;
    bool has_key3 = false;

    size_t pos = 1;

    while (pos < line.length())
    {
      while (pos < line.length() && line[pos] == ' ')
        ++pos;

      if (pos >= line.length() || line[pos] != ':')
      {
        break;
      }
      ++pos;

      std::string key;
      while (pos < line.length() && line[pos] != ' ')
      {
        key += line[pos];
        ++pos;
      }

      if (pos >= line.length())
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      ++pos;

      if (key == "key1")
      {
        std::string value;
        while (pos < line.length() && line[pos] != ':')
        {
          value += line[pos];
          ++pos;
        }

        if (!parse_complex(value, data.key1))
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        has_key1 = true;
      } else if (key == "key2")
      {
        std::string value;
        while (pos < line.length() && line[pos] != ':')
        {
          value += line[pos];
          ++pos;
        }

        if (!parse_rational(value, data.key2))
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        has_key2 = true;
      } else if (key == "key3")
      {
        if (pos >= line.length() || line[pos] != '"')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        ++pos;

        data.key3.clear();
        while (pos < line.length() && line[pos] != '"')
        {
          if (line[pos] == '\\' && pos + 1 < line.length())
          {
            ++pos;
          }
          data.key3 += line[pos];
          ++pos;
        }

        if (pos >= line.length() || line[pos] != '"')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        ++pos;

        if (pos >= line.length() || line[pos] != ':')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        has_key3 = true;
      } else
      {
        while (pos < line.length() && line[pos] != ':')
        {
          ++pos;
        }
      }

      if (pos < line.length() && line[pos] == ':')
      {
        ++pos;
      }
    }

    if (!has_key1 || !has_key2 || !has_key3)
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::ostream &operator<<(std::ostream &out, const DataStruct &data)
  {
    out << "(:key1 #c(" << data.key1.real() << " " << data.key1.imag() << "):"
        << "key2 (:N " << data.key2.first << ":D " << data.key2.second << ":):"
        << "key3 \"" << data.key3 << "\":)";
    return out;
  }

}

int main()
{
  return 0;
}