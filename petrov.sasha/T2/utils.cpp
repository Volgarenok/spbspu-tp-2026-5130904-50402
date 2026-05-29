#include "utils.hpp"

#include <iomanip>
#include <ioguard.hpp>

namespace petrov
{
  std::istream &operator>>(std::istream &in, DelimIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '\0';
    in >> c;
    if (!in || c != dest.exp) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, OctIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    IOGuard guard(in);
    in >> DelimIO{'0'} >> std::oct >> dest.ref;
    return in;
  }

  std::istream &operator>>(std::istream &in, HexIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    IOGuard guard(in);
    char zero = '\0';
    char x = '\0';
    in >> zero >> x;
    if (!in || zero != '0' || (x != 'x' && x != 'X')) {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> std::hex >> dest.ref;
    return in;
  }

  std::istream &operator>>(std::istream &in, StringIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return in >> std::quoted(dest.ref);
  }

  std::istream &operator>>(std::istream &in, DataStruct &dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    DataStruct temp{};
    bool got1 = false;
    bool got2 = false;
    bool got3 = false;
    in >> DelimIO{'('};
    while ((!got1 || !got2 || !got3) && in) {
      std::string label;
      in >> label;
      if (label == ":key1" && !got1) {
        in >> OctIO{temp.key1};
        if (in) {
          got1 = true;
        }
      } else if (label == ":key2" && !got2) {
        in >> HexIO{temp.key2};
        if (in) {
          got2 = true;
        }
      } else if (label == ":key3" && !got3) {
        in >> StringIO{temp.key3};
        if (in) {
          got3 = true;
        }
      } else {
        in.setstate(std::ios::failbit);
      }
    }
    in >> DelimIO{':'} >> DelimIO{')'};
    if (in && got1 && got2 && got3) {
      dest = temp;
    } else {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}

