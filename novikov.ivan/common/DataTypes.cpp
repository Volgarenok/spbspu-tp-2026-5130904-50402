#include "DataTypes.hpp"
#include <istream>
#include "IOGuard.hpp"
#include "DelimiterIO.hpp"

std::istream& novikov::operator>>(std::istream& in, dbl_lit&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  IOGuard g(in);

  double val = 0.0;
  in >> val >> uncase_sep{'d'};

  if (in)
  {
    dest.ref = val;
  }
  return in;
}

std::istream& novikov::operator>>(std::istream& in, cmp_lsp&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  IOGuard g(in);

  double real, imag;

  in >> sep{'#'} >> sep{'c'} >> sep{'('};
  in >> real;
  in >> sep{' '};
  in >> imag;
  in >> sep{')'};

  if (in)
  {
    dest.ref = std::complex< double >(real, imag);
  }
  return in;
}

std::istream& novikov::operator>>(std::istream& in, str&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  IOGuard g(in);
  in >> sep{'"'};
  if (!in)
  {
    return in;
  }
  return std::getline(in, dest.ref, '"');
}
