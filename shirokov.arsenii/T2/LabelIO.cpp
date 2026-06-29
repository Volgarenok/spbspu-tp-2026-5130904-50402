#include "LabelIO.hpp"
#include <istream>
#include "IOguard.hpp"

std::istream& shirokov::operator>>(std::istream& in, label&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  IOguard g(in);
  std::string data;
  for (size_t i = 0; i < 4; ++i)
  {
    char c = 0;
    if (in.get(c))
    {
      data += c;
    }
    else
    {
      break;
    }
  }

  char space = 0;
  if (in.get(space))
  {
    if (space != ' ')
    {
      in.putback(space);
      in.setstate(std::ios::failbit);
    }
  }

  if (!in)
  {
    return in;
  }

  DataType inputType = DataType::Unknown;
  if (data == dest.possibleLabels[0])
  {
    inputType = DataType::UllOct;
  }
  else if (data == dest.possibleLabels[1])
  {
    inputType = DataType::UllBin;
  }
  else if (data == dest.possibleLabels[2])
  {
    inputType = DataType::String;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  dest.used.push_back(inputType);
  return in;
}
