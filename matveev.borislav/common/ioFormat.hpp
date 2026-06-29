#ifndef IO_FORMAT_HPP
#define IO_FORMAT_HPP

#include <iosfwd>
#include <string>

namespace matveev
{
  struct DelimiterIO
  {
    char expected;
  };

  struct LabelIO
  {
    std::string expected;
  };

  struct UllLitIO
  {
    unsigned long long& ref;
  };

  struct ChrLitIO
  {
    char& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
  std::istream& operator>>(std::istream& in, UllLitIO&& dest);
  std::istream& operator>>(std::istream& in, ChrLitIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
}

#endif
