#include "DataStruct.hpp"
#include <iomanip>
#include <sstream>

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
    if (sci_str[e_pos + 1] != '+' && sci_str[e_pos + 1] != '-') {
      out << "+";
    }

    std::string exp_val = sci_str.substr(e_pos + 1);
    if (exp_val[0] == '+' || exp_val[0] == '-') {
      out << exp_val[0];
      exp_val = exp_val.substr(1);
    }
    long long exp_num = std::stoll(exp_val);
    out << exp_num;
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
  in >> c;
  if (c != ':') {
    in.setstate(std::ios_base::failbit);
    return in;
  }

  double k1 = 0.0;
  double k2 = 0.0;
  std::string k3 = "";
  bool has_k1 = false, has_k2 = false, has_k3 = false;

  for (size_t i = 0; i < 3; ++i) {
    std::string key = "";
    in >> c;
    if (c != 'k') {
      in.setstate(std::ios_base::failbit);
      return in;
    }
    key += c;
    in >> c;
    if (c != 'e') {
      in.setstate(std::ios_base::failbit);
      return in;
    }
    key += c;
    in >> c;
    if (c != 'y') {
      in.setstate(std::ios_base::failbit);
      return in;
    }
    key += c;
    in >> c;
    if (c != '1' && c != '2' && c != '3') {
      in.setstate(std::ios_base::failbit);
      return in;
    }
    key += c;

    in >> c;
    if (c != ' ') {
      in.setstate(std::ios_base::failbit);
      return in;
    }
  }
  return in;
}
