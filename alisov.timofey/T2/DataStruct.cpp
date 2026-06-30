#include "DataStruct.hpp"
#include <iomanip>
#include <sstream>

namespace alisov
{
  struct Delimiter
  {
    char expected;
  };

  std::istream &operator>>(std::istream &in, Delimiter &&dest)
  {
    std::istream::sentry guard(in);
    if (!guard) {
      return in;
    }
    char c = 0;
    in >> c;
    if (c != dest.expected) {
      in.setstate(std::ios_base::failbit);
    }
    return in;
  }
}

bool alisov::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2) {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.length() < rhs.key3.length();
}

std::ostream &alisov::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry guard(out);
  if (!guard) {
    return out;
  }

  out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
  out << ":key2 ";

  auto flags = out.flags();
  std::stringstream ss;
  ss << std::scientific << std::setprecision(2) << src.key2;
  std::string sci_str = ss.str();

  size_t e_pos = sci_str.find('e');
  if (e_pos != std::string::npos) {
    out << sci_str.substr(0, e_pos + 1);

    char sign = sci_str[e_pos + 1];
    std::string exp_val = sci_str.substr(e_pos + 1);

    if (sign == '+' || sign == '-') {
      out << sign;
      exp_val = exp_val.substr(1);
    } else {
      out << "+";
    }

    size_t start = exp_val.find_first_not_of('0');
    if (start == std::string::npos) {
      exp_val = "0";
    } else {
      exp_val = exp_val.substr(start);
    }

    if (exp_val.length() < 2) {
      out << "0" << exp_val;
    } else {
      out << exp_val;
    }
  } else {
    out << sci_str;
  }

  out.flags(flags);
  out << ":key3 " << std::quoted(src.key3) << ":)";
  return out;
}

std::istream &alisov::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry guard(in);
  if (!guard) {
    return in;
  }

  char c = 0;
  in >> c;
  if (c != '(') {
    in.setstate(std::ios_base::failbit);
    return in;
  }

  double k1 = 0.0;
  double k2 = 0.0;
  std::string k3 = "";
  bool has_k1 = false, has_k2 = false, has_k3 = false;

  for (size_t i = 0; i < 3; ++i) {
    in >> c;
    if (c != ':') {
      in.setstate(std::ios_base::failbit);
      return in;
    }

    std::string key = "";
    for (size_t j = 0; j < 4; ++j) {
      char key_char = 0;
      if (in >> key_char) {
        key += key_char;
      }
    }

    in >> std::noskipws >> c;
    std::skipws(in);
    if (c != ' ') {
      in.setstate(std::ios_base::failbit);
      return in;
    }

    if (key == "key1") {
      if (!(in >> k1))
        return in;
      in >> c;
      if (c != 'd' && c != 'D') {
        in.setstate(std::ios_base::failbit);
        return in;
      }
      has_k1 = true;
    } else if (key == "key2") {
      if (!(in >> k2))
        return in;
      has_k2 = true;
    } else if (key == "key3") {
      in >> std::ws;
      if (!(in >> std::quoted(k3)))
        return in;
      has_k3 = true;
    } else {
      in.setstate(std::ios_base::failbit);
      return in;
    }
  }

  in >> c;
  if (c != ':') {
    in.setstate(std::ios_base::failbit);
    return in;
  }
  in >> c;
  if (c != ')') {
    in.setstate(std::ios_base::failbit);
    return in;
  }

  if (has_k1 && has_k2 && has_k3) {
    dest.key1 = k1;
    dest.key2 = k2;
    dest.key3 = k3;
  } else {
    in.setstate(std::ios_base::failbit);
  }

  return in;
}
