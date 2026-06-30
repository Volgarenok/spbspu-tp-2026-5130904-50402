#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <complex>
#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include <IOGuard.hpp>

namespace novikov
{
  struct DataStruct
  {
    double key1;
    std::complex< double > key2;
    std::string key3;
  };

  enum DataType
  {
    DblLit,
    CmpLsp,
    String,
    Unknown
  };

  struct DblLitIO
  {
    double& ref;
  };

  struct CmpLspIO
  {
    std::complex< double >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct KeyIO
  {
    const std::vector< DataType >& used;
    DataStruct& input;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct DelimiterUnCaseIO
  {
    char exp;
  };

  struct LabelIO
  {
    std::vector< DataType >& used;
    const std::vector< std::string >& possibleLabels;
  };

  using sep = DelimiterIO;
  using uncase_sep = DelimiterUnCaseIO;
  using dbl_lit = DblLitIO;
  using cmp_lsp = CmpLspIO;
  using label = LabelIO;
  using str = StringIO;
  using key = KeyIO;

  std::istream& operator>>(std::istream&, sep&&);
  std::istream& operator>>(std::istream&, uncase_sep&&);
  std::istream& operator>>(std::istream&, dbl_lit&&);
  std::istream& operator>>(std::istream&, cmp_lsp&&);
  std::istream& operator>>(std::istream&, label&&);
  std::istream& operator>>(std::istream&, str&&);
  std::istream& operator>>(std::istream&, key&&);
  std::istream& operator>>(std::istream&, DataStruct&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);
  bool compare(const DataStruct&, const DataStruct&);
}

#endif
