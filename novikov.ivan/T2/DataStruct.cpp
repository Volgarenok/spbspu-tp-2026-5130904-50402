#include "DataStruct.hpp"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <limits>

std::istream& novikov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  IOGuard g(in);
  while (true)
  {
    in >> std::noskipws;

    DataStruct input;
    std::vector< DataType > used;
    const std::vector< std::string > possibleLabels{"key1", "key2", "key3"};

    in >> sep{'('} >> sep{':'};
    in >> label{used, possibleLabels} >> key{used, input};
    in >> sep{':'};
    in >> label{used, possibleLabels} >> key{used, input};
    in >> sep{':'};
    in >> label{used, possibleLabels} >> key{used, input};
    in >> sep{':'} >> sep{')'};

    if (in)
    {
      dest = input;
      return in;
    }

    in.clear();
    auto toIgnore = std::numeric_limits< std::streamsize >::max();
    in.ignore(toIgnore, '\n');
  }
}

std::istream& novikov::operator>>(std::istream& in, key&& dest)
{
  std::istream::sentry s(in);
  if (!s || dest.used.empty())
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  DataType currentType = dest.used.back();

  int count = 0;
  for (auto t : dest.used) {
    if (t == currentType) {
      count++;
    }
  }
  if (count > 1 || currentType == Unknown)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  switch (currentType)
  {
    case DblLit:
      in >> dbl_lit{dest.input.key1};
      break;
    case CmpLsp:
      in >> cmp_lsp{dest.input.key2};
      break;
    case String:
      in >> str{dest.input.key3};
      break;
    default:
      in.setstate(std::ios::failbit);
      break;
  }
  return in;
}

bool novikov::compare(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }

  double abs_lhs_k2 = std::abs(lhs.key2);
  double abs_rhs_k2 = std::abs(rhs.key2);
  if (abs_lhs_k2 != abs_rhs_k2)
  {
    return abs_lhs_k2 < abs_rhs_k2;
  }

  return lhs.key3.length() < rhs.key3.length();
}

std::istream& novikov::operator>>(std::istream& in, sep&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char c = 0;
  if (in.get(c))
  {
    if (c != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::istream& novikov::operator>>(std::istream& in, uncase_sep&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char c = 0;
  if (in.get(c))
  {
    if (std::toupper(static_cast< unsigned char >(c)) != std::toupper(static_cast< unsigned char >(dest.exp)))
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::istream& novikov::operator>>(std::istream& in, label&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }

  std::string data;
  if (!std::getline(in, data, ' '))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  DataType inputType = DataType::Unknown;

  if (data == dest.possibleLabels[0])
  {
    inputType = DataType::DblLit;
  }
  else if (data == dest.possibleLabels[1])
  {
    inputType = DataType::CmpLsp;
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

std::istream& novikov::operator>>(std::istream& in, dbl_lit&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }

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
  in >> sep{'"'};
  if (!in)
  {
    return in;
  }
  return std::getline(in, dest.ref, '"');
}

std::ostream& novikov::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }

  IOGuard g(out);
  out << std::fixed << std::setprecision(1);

  out << "(:key1 " << src.key1 << "d";
  out << ":key2 #c(" << src.key2.real() << " " << src.key2.imag() << ")";
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}
