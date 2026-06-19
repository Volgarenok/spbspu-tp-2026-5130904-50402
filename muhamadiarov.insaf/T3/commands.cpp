#include "commands.hpp"

namespace muhamadiarov
{
  double getArea(const Polygon& p)
  {
    if (p.points_.size() < 3)
    {
      return 0.0;
    }
    
    struct Accumulator
    {
      double operator()(const Point& a, const Point& b)
      {
        return static_cast<double>(a.x_ * b.y_ - a.y_ * b.x_);
      }
    };

    std::vector< double > crossProducts;
    std::transform(p.points_.begin(), p.points_.end() - 1, p.points_.begin() + 1,
      std::back_inserter(crossProducts), Accumulator());

    crossProducts.push_back(Accumulator()(p.points_.back(), p.points_.front()));
    const double sum = std::accumulate(crossProducts.begin(), crossProducts.end(), 0.0);
    return std::abs(sum) / 2.0;
  }

  double sumArea(const std::vector< Polygon >& p)
  {
    std::vector< double > areas;
    std::transform(p.begin(), p.end(), std::back_inserter(areas), getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  bool isOdd(const Polygon& p)
  {
    return (p.points_.size() % 2) != 0;
  }

  bool isEven(const Polygon& p)
  {
    return (p.points_.size() % 2) == 0;
  }

  bool isCountOfVertices(const Polygon& p, size_t n)
  {
    return n == p.points_.size();
  }
}

namespace muh = muhamadiarov;

void muh::area(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string command;
  if (!(in >> command))
  {
    throw std::runtime_error("error input");
  }

  out << std::fixed << std::setprecision(1);

  std::vector< Polygon > polygonsIf;
  if (command == "EVEN")
  {
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(polygonsIf), isEven);
    out << sumArea(polygonsIf) << '\n';
  }
  else if (command == "ODD")
  {
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(polygonsIf), isOdd);
    out << sumArea(polygonsIf) << '\n';
  }
  else if (command == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::logic_error("Empty collection");
    }
    out << sumArea(polygons) / static_cast< double >(polygons.size()) << '\n';
  }
  else
  {
    const size_t count = std::stoull(command);
    if (count <  3)
    {
      throw std::logic_error("Invalid argument");
    }

    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(polygonsIf),
      std::bind(isCountOfVertices, std::placeholders::_1, count));
    out << sumArea(polygonsIf) << '\n';
  }
}
