#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <iostream>
namespace saldaev
{
  struct DelimIO
  {
    char exp1;
    char exp2 = '\0';
    bool *matchedSecond = nullptr;
  };

  std::istream &operator>>(std::istream &in, DelimIO &&dest);
}

#endif
