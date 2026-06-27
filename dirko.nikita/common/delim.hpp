#ifndef DELIM_HPP
#define DELIM_HPP
#include <iostream>
namespace dirko
{
  struct DelimIO
  {
    char exp;
  };

  std::istream &operator>>(std::istream &in, DelimIO &&dest);
}
#endif
