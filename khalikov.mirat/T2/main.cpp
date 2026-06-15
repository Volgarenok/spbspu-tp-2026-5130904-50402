#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <iomanip>
#include <limits>

namespace khalikov {

	struct DataStruct {
	  double key1;
	  unsigned long long key2;
	  std::string key3;
	};

  struct DelimiterIO {
    char exp;
  };

  struct DoubleIO {
    double& ref;
  };

  struct UllIO {
    unsigned long long& ref;
  };

  struct StringIO {
    std::string& ref;
  };

  enum class Key {
    KEY1,
    KEY2,
    KEY3
  };

  struct KeyIO {
    Key& ref;
  };

  class IoGuard {
    public:
      explicit IoGuard(std::basic_ios< char >& s);
      ~IoGuard();
    private:
      std::basic_ios< char >& s_;
      std::streamsize width_;
      std::streamsize precision_;
      std::basic_ios< char >::fmtflags fmt_;
      char fill_;
   };

  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, UllIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, KeyIO&& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& src);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
}

int main() {
  std::vector< khalikov::DataStruct > data;
  {
    using iit_t = std::istream_iterator< khalikov::DataStruct >;
    while (!std::cin.eof()) {
      std::copy(iit_t(std::cin), iit_t(), std::back_inserter(data));
      if (std::cin.fail() && !std::cin.eof()) {
        std::cin.clear();
        auto toIgnore = std::numeric_limits< std::streamsize >::max();
        std::cin.ignore(toIgnore, '\n');
      }
    }
  }
  std::sort(data.begin(), data.end());
  {
    using oit_t = std::ostream_iterator<khalikov::DataStruct>;
    std::copy(data.begin(), data.end(), oit_t(std::cout, "\n"));
  }
}

std::istream& khalikov::operator>>(std::istream& in, KeyIO&& dest) {
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  std::string curr = "";
  in >> curr;
  if (curr == "key1") {
    dest.ref = Key::KEY1;
  }
  else if (curr == "key2") {
    dest.ref = Key::KEY2;
  }
  else if (curr == "key3") {
    dest.ref = Key::KEY3;
  }
  else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& khalikov::operator>>(std::istream& in, StringIO&& dest) {
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& khalikov::operator>>(std::istream& in, UllIO&& dest) {
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  IoGuard fmtguard(in);
  in >> std::oct >> dest.ref >> std::dec;
  return in;
}

std::istream& khalikov::operator>>(std::istream& in, DoubleIO&& dest) {
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  in >> dest.ref;
  char suf = '0';
  in >> suf;
  if (in && (suf != 'd' && suf != 'D')) {
    in.setstate(std::ios::failbit);
  }
  return in;
}


std::istream& khalikov::operator>>(std::istream& in, DelimiterIO&& dest) {
  std::istream::sentry s(in);
  if (!s) {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& khalikov::operator<<(std::ostream& out, const DataStruct& src) {
  std::ostream::sentry s(out);
  if (!s) {
    return out;
  }
  IoGuard fmtguard(out);
  out << "(:";
  out << "key1 " << std::fixed << std::setprecision(1) << src.key1 << 'd';
  out << ":key2 " << std::oct << std::showbase << src.key2;
  out << ":key3 \"" << src.key3 << '"';
  out << ":)";
  return out;
}

khalikov::IoGuard::IoGuard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

khalikov::IoGuard::~IoGuard() {
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}


