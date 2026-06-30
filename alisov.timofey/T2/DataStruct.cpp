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
  return out;
}
