#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iosfwd>
#include <complex>
#include <utility>
#include <string>

namespace chernikov {

  struct DataStruct
  {
    std::complex< double > key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };

  double key2_to_double(const std::pair< long long, unsigned long long > &p);

  bool compare_data(const DataStruct &a, const DataStruct &b);

  std::istream &operator>>(std::istream &in, DataStruct &data);

  std::ostream &operator<<(std::ostream &out, const DataStruct &data);

}

#endif
