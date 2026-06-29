#include <ioFormat.hpp>

#include <istream>
#include <string>

#include <ioGuard.hpp>

namespace
{
  bool isUnsignedSuffix(char c)
  {
    return c == 'u' || c == 'U';
  }

  bool isLongSuffix(char c)
  {
    return c == 'l' || c == 'L';
  }
}

std::istream& matveev::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  char c = 0;
  in >> c;

  if (in && c != dest.expected)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& matveev::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  for (char symbol : dest.expected)
  {
    char c = 0;
    in >> c;

    if (!in || c != symbol)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  return in;
}

std::istream& matveev::operator>>(std::istream& in, UllLitIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  in >> std::ws;

  if (in.peek() == '-')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  unsigned long long value = 0;
  in >> value;

  if (!in)
  {
    return in;
  }

  IOGuard guard(in);
  in >> std::noskipws;

  char first = 0;
  char second = 0;
  char third = 0;

  in >> first >> second >> third;

  if (!in || !isUnsignedSuffix(first) || !isLongSuffix(second) || !isLongSuffix(third))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  dest.ref = value;
  return in;
}

std::istream& matveev::operator>>(std::istream& in, ChrLitIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  IOGuard guard(in);

  in >> DelimiterIO{ '\'' };

  if (!in)
  {
    return in;
  }

  char value = 0;
  in >> std::noskipws >> value;

  if (!in)
  {
    return in;
  }

  in >> DelimiterIO{ '\'' };

  if (in)
  {
    dest.ref = value;
  }

  return in;
}

std::istream& matveev::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO{ '"' };

  if (!in)
  {
    return in;
  }

  std::string value;
  std::getline(in, value, '"');

  if (in)
  {
    dest.ref = value;
  }

  return in;
}
