#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <iostream>
namespace karpovich
{
  struct DelimIO
  {
    char exp1;
    char exp2 = '\0';
  };
  std::istream &operator>>(std::istream &in, DelimIO &&dest);
}

#endif
