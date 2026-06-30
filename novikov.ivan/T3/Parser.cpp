#include "Parser.hpp"
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>
#include "IOGuard.hpp"
#include "DelimiterIO.hpp"

std::istream& novikov::operator>>(std::istream& in, Point& pt)
{
  in >> DelimiterIO{' '} >> DelimiterIO{'('} >> pt.x >> DelimiterIO{';'} >> pt.y >> DelimiterIO{')'};
  return in;
}

std::istream& novikov::operator>>(std::istream& in, PolygonIO&& dest)
{
  IOGuard g(in);
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

  std::vector< novikov::Point > temp_points;
  temp_points.reserve(count);

  std::copy_n(std::istream_iterator< novikov::Point >(in), count, std::back_inserter(temp_points));

  if (in)
  {
    dest.p.points = std::move(temp_points);
  }
  return in;
}

std::istream& novikov::operator>>(std::istream& in, Polygon& p)
{
  return in >> PolygonIO{p};
}

std::istream& novikov::operator>>(std::istream& in, novikov::Line& dest)
{
  std::getline(in, dest.lineText);
  return in;
}

novikov::PolygonLineInserter::PolygonLineInserter(novikov::plg_t& containerVec):
    container(containerVec)
{}

novikov::PolygonLineInserter& novikov::PolygonLineInserter::operator=(const novikov::Line& line)
{
  std::istringstream iss(line.lineText);
  novikov::Polygon poly;
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

novikov::PolygonLineInserter& novikov::PolygonLineInserter::operator*()
{
  return *this;
}

novikov::PolygonLineInserter& novikov::PolygonLineInserter::operator++()
{
  return *this;
}

novikov::PolygonLineInserter& novikov::PolygonLineInserter::operator++(int)
{
  return *this;
}

novikov::plg_t novikov::parse(std::istream& in)
{
  novikov::plg_t data;
  std::copy(std::istream_iterator< Line >(in), std::istream_iterator< Line >(), PolygonLineInserter{data});
  return data;
}
