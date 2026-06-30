#include "io_structs.hpp"
#include <istream>
#include <string>

std::istream &chernikov::operator>>(std::istream &in, DelimIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '\0';
  in >> c;
  if (!in || c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream &chernikov::operator>>(std::istream &in, LabelIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  for (size_t i = 0; i < dest.exp.size(); ++i)
  {
    char c = '\0';
    in >> c;
    if (!in || c != dest.exp[i])
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  return in;
}

std::istream &chernikov::operator>>(std::istream &in, CmpLspIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  double real = 0.0;
  double imag = 0.0;

  in >> LabelIO{"#c("} >> real >> imag >> DelimIO{')'};

  if (in)
  {
    dest.ref = std::complex< double >(real, imag);
  }

  return in;
}

std::istream &chernikov::operator>>(std::istream &in, RatLspIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  long long num = 0;
  unsigned long long den = 0;

  in >> LabelIO{"(:N"} >> num >> LabelIO{":D"} >> den >> LabelIO{":)"};

  if (in && den != 0)
  {
    dest.ref = std::make_pair(num, den);
  } else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream &chernikov::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  return in >> std::quoted(dest.ref);
}
