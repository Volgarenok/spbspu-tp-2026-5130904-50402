#include "parser.hpp"
#include <cctype>
#include <limits>
#include <vector>
#include "DelimiterIO.hpp"
#include "IOguard.hpp"

std::istream& shirokov::operator>>(std::istream& in, Point& pt)
{
  in >> DelimiterIO{'('} >> pt.x >> DelimiterIO{';'} >> pt.y >> DelimiterIO{')'};
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, Polygon& polygon)
{
  IOguard g(in);
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }

  size_t count = 0;
  if (!(in >> count))
  {
    return in;
  }

  if (count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > temp_points;
  temp_points.reserve(count);

  for (size_t i = 0; i < count; ++i)
  {
    while (in.peek() == ' ' || in.peek() == '\t')
    {
      in.ignore();
    }

    if (in.peek() == '\n' || in.peek() == EOF)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    Point pt;
    if (in >> pt)
    {
      temp_points.push_back(pt);
    }
    else
    {
      return in;
    }
  }

  polygon.points = std::move(temp_points);
  return in;
}

shirokov::plg_t shirokov::parse(std::istream& in)
{
  plg_t data;
  while (!in.eof())
  {
    Polygon p;
    if (in >> p)
    {
      bool hasTrailingGarbage = false;
      while (in.peek() != '\n' && in.peek() != EOF)
      {
        char ch = 0;
        in.get(ch);
        if (std::isspace(static_cast< unsigned char >(ch)) == 0)
        {
          hasTrailingGarbage = true;
        }
      }

      if (!hasTrailingGarbage)
      {
        data.push_back(p);
      }
    }
    else
    {
      if (in.eof())
      {
        break;
      }

      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return data;
}
