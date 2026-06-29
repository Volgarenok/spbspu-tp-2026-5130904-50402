#include "data_struct.hpp"
#include "io_structs.hpp"
#include <istream>
#include <ostream>
#include <cmath>
#include <string>

double chernikov::key2_to_double(const std::pair< long long, unsigned long long > &p)
{
  return static_cast< double >(p.first) / static_cast< double >(p.second);
}

bool chernikov::compare_data(const DataStruct &a, const DataStruct &b)
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

std::istream &chernikov::operator>>(std::istream &in, DataStruct &data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c;
  if (!(in >> c) || c != '(')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> c) || c != ':')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  bool has_key1 = false;
  bool has_key2 = false;
  bool has_key3 = false;

  while (in.good())
  {
    std::string key;
    char next_char = static_cast< char >(in.peek());

    if (next_char == ':')
    {
      in >> c;
      in >> c;
      break;
    }

    while (in.get(c) && c != ' ' && c != ':')
    {
      key += c;
    }

    if (c == ' ')
    {
      if (!(in >> c) || c != ':')
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }

    if (key == "key1")
    {
      if (has_key1)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> ComplexIO(data.key1);
      if (!in)
      {
        return in;
      }
      has_key1 = true;
    } else if (key == "key2")
    {
      if (has_key2)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> RationalIO(data.key2);
      if (!in)
      {
        return in;
      }
      has_key2 = true;
    } else if (key == "key3")
    {
      if (has_key3)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> StringIO(data.key3);
      if (!in)
      {
        return in;
      }
      has_key3 = true;
    } else
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    next_char = static_cast< char >(in.peek());
    if (next_char == ':')
    {
      in >> c;
    } else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  if (!has_key1 || !has_key2 || !has_key3)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream &chernikov::operator<<(std::ostream &out, const DataStruct &data)
{
  out << "(:key1 #c(" << data.key1.real() << " " << data.key1.imag() << "):"
      << "key2 (:N " << data.key2.first << ":D " << data.key2.second << ":):"
      << "key3 \"" << data.key3 << "\":)";
  return out;
}
