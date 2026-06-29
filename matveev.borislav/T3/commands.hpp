#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <cstddef>
#include <iosfwd>
#include <vector>

#include "geometry.hpp"

namespace matveev
{
  using data_t = std::vector< Polygon >;

  class Contexts
  {
  public:
    explicit Contexts(const data_t& base);

    const data_t& current() const;
    std::size_t level() const;
    void push(const data_t& context);
    bool pop();

  private:
    std::vector< data_t > stack_;
  };

  bool doArea(std::istream& in, std::ostream& out, Contexts& contexts);
  bool doCount(std::istream& in, std::ostream& out, Contexts& contexts);
  bool doMax(std::istream& in, std::ostream& out, Contexts& contexts);
  bool doMin(std::istream& in, std::ostream& out, Contexts& contexts);
  bool doInFrame(std::istream& in, std::ostream& out, Contexts& contexts);
  bool doIntersections(std::istream& in, std::ostream& out, Contexts& contexts);
  bool doContext(std::istream& in, std::ostream& out, Contexts& contexts);
  bool doPopContext(std::istream& in, std::ostream& out, Contexts& contexts);
  bool doLevel(std::istream& in, std::ostream& out, Contexts& contexts);
}

#endif
