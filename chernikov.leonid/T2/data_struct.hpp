#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iosfwd>
#include <string>

namespace chernikov {

  struct DataStruct
  {
    std::string key1;
    double key2;
    std::string key3;
  };

  bool compare_data(const DataStruct &a, const DataStruct &b);

  std::istream &operator>>(std::istream &in, DataStruct &data);

  std::ostream &operator<<(std::ostream &out, const DataStruct &data);

}

#endif
