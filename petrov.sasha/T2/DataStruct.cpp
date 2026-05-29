#include "DataStruct.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <istream>
#include <limits>
#include <string>

namespace petrov
{
  class IOGuard
  {
  public:
    explicit IOGuard(std::ios &stream):
      stream_(stream),
      flags_(stream.flags()),
      fill_(stream.fill())
    {}

    ~IOGuard()
    {
      stream_.flags(flags_);
      stream_.fill(fill_);
    }

  private:
    std::ios &stream_;
    std::ios::fmtflags flags_;
    char fill_;
  };
}

