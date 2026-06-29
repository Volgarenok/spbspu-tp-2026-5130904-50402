#include "parser.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include "DelimiterIO.hpp"
#include "IOguard.hpp"

std::istream& shirokov::operator>>(std::istream& in, Point& pt)
{
  in >> DelimiterIO{' '} >> DelimiterIO{'('} >> pt.x >> DelimiterIO{';'} >> pt.y >> DelimiterIO{')'};
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, PolygonIO&& dest)
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

  in >> std::noskipws;

  std::vector< Point > temp_points;
  temp_points.reserve(count);

  std::copy_n(std::istream_iterator< Point >(in), count, std::back_inserter(temp_points));

  if (in)
  {
    dest.p.points = std::move(temp_points);
  }
  return in;
}

std::istream& shirokov::operator>>(std::istream& in, Polygon& p)
{
  return in >> PolygonIO{p};
}

std::istream& shirokov::operator>>(std::istream& in, Line& dest)
{
  std::getline(in, dest.lineText);
  return in;
}

shirokov::PolygonLineInserter::PolygonLineInserter(plg_t& containerVec):
  container(containerVec)
{}

shirokov::PolygonLineInserter& shirokov::PolygonLineInserter::operator=(const Line& line)
{
  std::istringstream iss(line.lineText);
  Polygon poly;
  if (iss >> poly)
  {
    char ch = 0;
    if (!(iss >> ch))
    {
      container.push_back(poly);
    }
  }
  return *this;
}

shirokov::PolygonLineInserter& shirokov::PolygonLineInserter::operator*()
{
  return *this;
}

shirokov::PolygonLineInserter& shirokov::PolygonLineInserter::operator++()
{
  return *this;
}

shirokov::PolygonLineInserter& shirokov::PolygonLineInserter::operator++(int)
{
  return *this;
}

shirokov::plg_t shirokov::parse(std::istream& in)
{
  plg_t data;
  std::copy(std::istream_iterator< Line >(in), std::istream_iterator< Line >(), PolygonLineInserter{data});
  return data;
}
