#include "lab_io.hpp"
#include <delim.hpp>
#include <ioguard.hpp>
#include <iomanip>

std::istream &dirko::operator>>(std::istream &in, LabelIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  for (size_t i = 0; i < dest.exp.size(); ++i) {
    char c = '\0';
    in >> c;
    if (!in || c != dest.exp[i]) {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  return in;
}

std::istream &dirko::operator>>(std::istream &in, CharIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  IOguard guard(in);
  in >> DelimIO{'\''} >> dest.ref >> DelimIO{'\''};
  return in;
}

std::istream &dirko::operator>>(std::istream &in, CompIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  IOguard guard(in);
  double real = 0, imag = 0;
  in >> LabelIO{"#c("} >> real >> imag >> DelimIO{')'};
  dest.ref = {real, imag};
  return in;
}

std::istream &dirko::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> std::quoted(dest.ref);
}

bool dirko::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2) {
    return std::abs(lhs.key2) < std::abs(rhs.key2);
  }
  return lhs.key3.length() < rhs.key3.length();
}

std::istream &dirko::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  DataStruct tmp;
  bool got1 = false, got2 = false, got3 = false;
  in >> DelimIO{'('};
  while (in && (!got1 || !got2 || !got3)) {
    std::string label;
    in >> label;
    if (label == ":key1" && !got1) {
      in >> CharIO{tmp.key1};
      if (in) {
        got1 = true;
      }
    } else if (label == ":key2" && !got2) {
      in >> CompIO{tmp.key2};
      if (in) {
        got2 = true;
      }
    } else if (label == ":key3" && !got3) {
      in >> StringIO{tmp.key3};
      if (in) {
        got3 = true;
      }
    } else {
      in.setstate(std::ios::failbit);
    }
  }
  in >> LabelIO{":)"};
  if (in && got1 && got2 && got3) {
    dest = tmp;
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream &dirko::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOguard guard(out);
  out << "(:" << "key1 " << '\'' << src.key1 << "':";
  out << "key2 " << "#c(" << std::fixed << std::setprecision(1) << src.key2.real() << ' ' << src.key2.imag() << "):";
  out << "key3 " << std::quoted(src.key3) << ":)";
  return out;
}
