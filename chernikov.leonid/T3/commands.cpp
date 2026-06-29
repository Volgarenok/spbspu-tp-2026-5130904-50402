#include "commands.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <string>

namespace {

  double getArea(const chernikov::Polygon &p)
  {
    return chernikov::computeArea(p);
  }

  size_t getVertexCount(const chernikov::Polygon &p)
  {
    return p.points.size();
  }

  void printAreaCommand(const std::vector< chernikov::Polygon > &polygons,
                        std::function< bool(const chernikov::Polygon &) > filter)
  {
    std::vector< double > filteredAreas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(filteredAreas),
                   [&filter](const chernikov::Polygon &p) {
                     return filter(p) ? getArea(p) : 0.0;
                   });

    double total = std::accumulate(filteredAreas.begin(), filteredAreas.end(), 0.0);
    std::cout << std::fixed << std::setprecision(1) << total << "\n";
  }

  bool isOdd(size_t n)
  {
    return n % 2 != 0;
  }

  bool isEven(size_t n)
  {
    return n % 2 == 0;
  }

  void handleAreaCommand(const std::vector< chernikov::Polygon > &polygons, std::istringstream &stream)
  {
    std::string subcommand;
    stream >> subcommand;

    if (subcommand == "EVEN")
    {
      if (std::none_of(polygons.begin(), polygons.end(), [](const chernikov::Polygon &p) {
            return isEven(getVertexCount(p));
          }))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
      printAreaCommand(polygons, [](const chernikov::Polygon &p) {
        return isEven(getVertexCount(p));
      });
    } else if (subcommand == "ODD")
    {
      if (std::none_of(polygons.begin(), polygons.end(), [](const chernikov::Polygon &p) {
            return isOdd(getVertexCount(p));
          }))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
      printAreaCommand(polygons, [](const chernikov::Polygon &p) {
        return isOdd(getVertexCount(p));
      });
    } else if (subcommand == "MEAN")
    {
      if (polygons.empty())
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      std::vector< double > areas;
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);

      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      double mean = total / static_cast< double >(polygons.size());
      std::cout << std::fixed << std::setprecision(1) << mean << "\n";
    } else
    {
      int num = 0;
      stream.clear();
      stream.seekg(0);
      stream >> num;

      if (!stream || num < 3)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      if (std::none_of(polygons.begin(), polygons.end(), [num](const chernikov::Polygon &p) {
            return static_cast< int >(getVertexCount(p)) == num;
          }))
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      printAreaCommand(polygons, [num](const chernikov::Polygon &p) {
        return static_cast< int >(getVertexCount(p)) == num;
      });
    }
  }

  void handleMaxCommand(const std::vector< chernikov::Polygon > &polygons, std::istringstream &stream)
  {
    if (polygons.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    std::string subcommand;
    stream >> subcommand;

    if (subcommand == "AREA")
    {
      auto it = std::max_element(polygons.begin(), polygons.end(),
                                 [](const chernikov::Polygon &a, const chernikov::Polygon &b) {
                                   return getArea(a) < getArea(b);
                                 });
      std::cout << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
    } else if (subcommand == "VERTEXES")
    {
      auto it = std::max_element(polygons.begin(), polygons.end(),
                                 [](const chernikov::Polygon &a, const chernikov::Polygon &b) {
                                   return getVertexCount(a) < getVertexCount(b);
                                 });
      std::cout << getVertexCount(*it) << "\n";
    } else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  void handleMinCommand(const std::vector< chernikov::Polygon > &polygons, std::istringstream &stream)
  {
    if (polygons.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    std::string subcommand;
    stream >> subcommand;

    if (subcommand == "AREA")
    {
      auto it = std::min_element(polygons.begin(), polygons.end(),
                                 [](const chernikov::Polygon &a, const chernikov::Polygon &b) {
                                   return getArea(a) < getArea(b);
                                 });
      std::cout << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
    } else if (subcommand == "VERTEXES")
    {
      auto it = std::min_element(polygons.begin(), polygons.end(),
                                 [](const chernikov::Polygon &a, const chernikov::Polygon &b) {
                                   return getVertexCount(a) < getVertexCount(b);
                                 });
      std::cout << getVertexCount(*it) << "\n";
    } else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  void handleCountCommand(const std::vector< chernikov::Polygon > &polygons, std::istringstream &stream)
  {
    std::string subcommand;
    stream >> subcommand;

    if (subcommand == "EVEN")
    {
      auto count = std::count_if(polygons.begin(), polygons.end(), [](const chernikov::Polygon &p) {
        return isEven(getVertexCount(p));
      });
      std::cout << count << "\n";
    } else if (subcommand == "ODD")
    {
      auto count = std::count_if(polygons.begin(), polygons.end(), [](const chernikov::Polygon &p) {
        return isOdd(getVertexCount(p));
      });
      std::cout << count << "\n";
    } else
    {
      int num = 0;
      stream.clear();
      stream.seekg(0);
      stream >> num;

      if (!stream || num < 3)
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }

      auto count = std::count_if(polygons.begin(), polygons.end(), [num](const chernikov::Polygon &p) {
        return static_cast< int >(getVertexCount(p)) == num;
      });
      std::cout << count << "\n";
    }
  }

  void handlePermsCommand(const std::vector< chernikov::Polygon > &polygons, std::istringstream &stream)
  {
    chernikov::Polygon target;
    stream >> target;

    if (!stream || target.points.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    auto count = std::count_if(polygons.begin(), polygons.end(), [&target](const chernikov::Polygon &p) {
      return chernikov::isPermutationOf(p, target);
    });
    std::cout << count << "\n";
  }

  void handleMaxseqCommand(const std::vector< chernikov::Polygon > &polygons, std::istringstream &stream)
  {
    chernikov::Polygon target;
    stream >> target;

    if (!stream || target.points.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    size_t maxSeq = 0;
    size_t currentSeq = 0;

    auto updateSeq = [&maxSeq, &currentSeq, &target](const chernikov::Polygon &p) {
      if (p == target)
      {
        ++currentSeq;
        if (currentSeq > maxSeq)
        {
          maxSeq = currentSeq;
        }
      } else
      {
        currentSeq = 0;
      }
    };

    std::for_each(polygons.begin(), polygons.end(), updateSeq);
    std::cout << maxSeq << "\n";
  }

  void handleRmechoCommand(std::vector< chernikov::Polygon > &polygons, std::istringstream &stream)
  {
    chernikov::Polygon target;
    stream >> target;

    if (!stream || target.points.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    std::vector< chernikov::Polygon > result;
    result.reserve(polygons.size());

    int removed = 0;

    for (size_t i = 0; i < polygons.size(); ++i)
    {
      bool isCurrentTarget = (polygons[i] == target);

      if (isCurrentTarget && !result.empty() && (result.back() == target))
      {
        ++removed;
      } else
      {
        result.push_back(polygons[i]);
      }
    }

    polygons = std::move(result);
    std::cout << removed << "\n";
  }

  void handleRectsCommand(const std::vector< chernikov::Polygon > &polygons)
  {
    auto count = std::count_if(polygons.begin(), polygons.end(), chernikov::isRectangle);
    std::cout << count << "\n";
  }

  void handleRightshapesCommand(const std::vector< chernikov::Polygon > &polygons)
  {
    auto count = std::count_if(polygons.begin(), polygons.end(), chernikov::hasRightAngle);
    std::cout << count << "\n";
  }

}

void chernikov::processCommand(const std::string &line, std::vector< Polygon > &polygons)
{
  std::istringstream stream(line);
  std::string command;
  stream >> command;

  if (command == "AREA")
  {
    handleAreaCommand(polygons, stream);
  } else if (command == "MAX")
  {
    handleMaxCommand(polygons, stream);
  } else if (command == "MIN")
  {
    handleMinCommand(polygons, stream);
  } else if (command == "COUNT")
  {
    handleCountCommand(polygons, stream);
  } else if (command == "PERMS")
  {
    handlePermsCommand(polygons, stream);
  } else if (command == "MAXSEQ")
  {
    handleMaxseqCommand(polygons, stream);
  } else if (command == "RMECHO")
  {
    handleRmechoCommand(polygons, stream);
  } else if (command == "RECTS")
  {
    handleRectsCommand(polygons);
  } else if (command == "RIGHTSHAPES")
  {
    handleRightshapesCommand(polygons);
  } else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}
