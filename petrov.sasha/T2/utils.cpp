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
}

