#include "DataStruct.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <istream>
#include <limits>
#include <string>

namespace petrov
{
  const char OPEN_PAREN = '(';
  const char CLOSE_PAREN = ')';
  const char COLON = ':';

  static bool isOctDigit(char c) {
    return c >= '0' && c <= '7';
  }

  static bool isHexDigit(char c)
  {
    bool tmp = std::isdigit(static_cast<unsigned char>(c));
    tmp = tmp || (c >= 'a' && c <= 'f');
    tmp = tmp || (c >= 'A' && c <= 'F');
    return tmp;
  }
}

