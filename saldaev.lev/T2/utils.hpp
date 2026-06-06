#ifndef UTILS_HPP
#define UTILS_HPP
#include <complex>
#include <iomanip>
#include <string>

namespace saldaev
{
  class IOguard
  {
  public:
    explicit IOguard(std::basic_ios< char > &s);
    ~IOguard();

  private:
    std::basic_ios< char > &s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };

  IOguard::IOguard(std::basic_ios< char > &s):
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  IOguard::~IOguard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }

  struct DelimiterIO
  {
    char exp;
  };

  struct StringIO
  {
    std::string &ref;
  };

  struct LabelIO
  {
    std::string exp;
  };

  struct ULL_bin
  {
    unsigned long long &ref;
  };

  struct CMP
  {
    std::complex< double > &ref;
  };

  struct Data
  {
    unsigned long long key1;
    std::complex< double > key2;
    std::string key3;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
  std::istream &operator>>(std::istream &in, LabelIO &&dest);
  std::istream &operator>>(std::istream &in, ULL_bin &&dest);
  std::istream &operator>>(std::istream &in, CMP &&dest);
  std::istream &operator>>(std::istream &in, Data &&dest);
  std::ostream &operator<<(std::ostream &out, const Data &dest);

  bool operator<(const Data &lhs, const Data &rhs);
}

std::istream &saldaev::operator>>(std::istream &in, DelimiterIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &saldaev::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream &saldaev::operator>>(std::istream &in, LabelIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string data = "";
  if ((in >> StringIO{data}) && (data != dest.exp)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &saldaev::operator>>(std::istream &in, ULL_bin &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c1 = '\0';
  char c2 = '\0';
  in >> c1 >> c2;
  if (!in || c1 != '0' || (c2 != 'b' && c2 != 'B')) {
    in.setstate(std::ios::failbit);
    return in;
  }
  char c = '\0';
  std::string numBin;
  while (in.get(c)) {
    if (c == '0' || c == '1') {
      numBin.push_back(c);
    } else {
      in.putback(c);
      break;
    }
  }
  if (numBin.empty()) {
    in.setstate(std::ios::failbit);
    return in;
  }
  try {
    dest.ref = std::stoull(numBin);
  } catch (const std::out_of_range &) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &saldaev::operator>>(std::istream &in, CMP &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  IOguard guard(in);

  double real = 0, imag = 0;
  in >> DelimiterIO{'#'} >> DelimiterIO{'c'} >> DelimiterIO{'('} >> real >> imag >> DelimiterIO{')'};
  dest.ref = {real, imag};
  return in;
}

std::istream &saldaev::operator>>(std::istream &in, Data &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  Data tmp;
  bool got1 = false, got2 = false, got3 = false;
  in >> DelimiterIO{'('};
  while (in && (!got1 || !got2 || !got3)) {
    std::string label;
    in >> label;
    if (label == ":key1" && !got1) {
      in >> ULL_bin{tmp.key1};
      if (in) {
        got1 = true;
      }
    } else if (label == ":key2" && !got2) {
      in >> CMP{tmp.key2};
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

std::ostream &saldaev::operator<<(std::ostream &out, const Data &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOguard guard(out);
  out << "(:" << "key1 " << '0b' << src.key1 << "':";
  out << "key2 " << "#c(" << std::fixed << std::setprecision(1) << src.key2.real() << ' ' << src.key2.imag() << "):";
  out << "key3 " << std::quoted(src.key3) << ":)";
  return out;
}

bool saldaev::operator<(const Data &lhs, const Data &rhs)
{
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2) {
    return std::abs(lhs.key2) < std::abs(rhs.key2);
  }
  return lhs.key3.length() < rhs.key3.length();
}

#endif
