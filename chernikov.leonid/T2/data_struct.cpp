#include "data_struct.hpp"
#include "io_structs.hpp"
#include <istream>
#include <ostream>
#include <string>

bool chernikov::compare_data(const DataStruct &a, const DataStruct &b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }

  if (a.key2 != b.key2)
  {
    return a.key2 < b.key2;
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
      in >> StringIO(data.key1);
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
      in >> DoubleIO(data.key2);
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
  out << "(:key1 \"" << data.key1 << "\":"
      << "key2 " << data.key2 << ":"
      << "key3 \"" << data.key3 << "\":)";
  return out;
}
