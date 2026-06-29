#ifndef DELIMITER_IO_HPP
#define DELIMITER_IO_HPP
#include <istream>

namespace shirokov
{
  struct DelimiterIO
  {
    char exp;
  };

  struct DelimiterUnCaseIO
  {
    char exp;
  };

  using sep = DelimiterIO;
  using uncase_sep = DelimiterUnCaseIO;

  std::istream& operator>>(std::istream&, sep&&);
  std::istream& operator>>(std::istream&, uncase_sep&&);
}

#endif
