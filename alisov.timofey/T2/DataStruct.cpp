#include "DataStruct.hpp"
#include <iomanip>

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
  return out;
}
