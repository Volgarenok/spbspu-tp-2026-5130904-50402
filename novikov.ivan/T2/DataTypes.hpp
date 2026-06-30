#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP
#include "DataStruct.hpp"

namespace novikov
{
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

  using dbl_lit = DblLitIO;
  using cmp_lsp = CmpLspIO;
  using str = StringIO;

  std::istream& operator>>(std::istream&, dbl_lit&&);
  std::istream& operator>>(std::istream&, cmp_lsp&&);
  std::istream& operator>>(std::istream&, str&&);
}

#endif
