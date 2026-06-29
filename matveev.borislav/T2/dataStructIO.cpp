#include <istream>
#include <ostream>
#include <string>

#include "dataStructIO.hpp"
#include <ioFormat.hpp>

std::istream& matveev::operator>>(std::istream& in, DataStruct& data)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  DataStruct input;
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;

  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

  for (int i = 0; in && i < 3; ++i)
  {
    in >> LabelIO{ "key" };

    char number = 0;
    in >> number;

    if (number == '1' && !hasKey1)
    {
      in >> UllLitIO{ input.key1 };
      hasKey1 = true;
    }
    else if (number == '2' && !hasKey2)
    {
      in >> ChrLitIO{ input.key2 };
      hasKey2 = true;
    }
    else if (number == '3' && !hasKey3)
    {
      in >> StringIO{ input.key3 };
      hasKey3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }

    in >> DelimiterIO{ ':' };
  }

  in >> DelimiterIO{ ')' };

  if (in && hasKey1 && hasKey2 && hasKey3)
  {
    data = input;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream& matveev::operator<<(std::ostream& out, const DataStruct& data)
{
  std::ostream::sentry sentry(out);

  if (!sentry)
  {
    return out;
  }

  out << "(:key1 " << data.key1 << "ull";
  out << ":key2 '" << data.key2 << "'";
  out << ":key3 \"" << data.key3 << "\"";
  out << ":)";

  return out;
}
