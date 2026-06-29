#include "DataStruct.hpp"
#include <iomanip>
#include <cctype>

namespace afanasev
{
  std::istream & operator>>(std::istream & in, StringMatch && str)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }

    for (char expected : str.exp)
    {
      char c = 0;
      in >> c;
      if (!in || c != expected)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    return in;
  }

  std::istream & operator>>(std::istream & in, ULLIn && str)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }

    unsigned long long val;
    if (!(in >> val))
    {
      return in;
    }

    char u, l1, l2;
    if (!(in >> u >> l1 >> l2))
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    u = static_cast< char >(std::tolower(static_cast< unsigned char >(u)));
    l1 = static_cast< char >(std::tolower(static_cast< unsigned char >(l1)));
    l2 = static_cast< char >(std::tolower(static_cast< unsigned char >(l2)));

    if (u == 'u' && l1 == 'l' && l2 == 'l')
    {
      str.num = val;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::istream & operator>>(std::istream & in, RacionalNum && str)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }

    long long numer = 0;
    unsigned long long denom = 1;
    in >> DelimIO{'('} >> DelimIO{':'} >> DelimIO{'N'};
    in >> std::ws >> numer;
    in >> DelimIO{':'} >> DelimIO{'D'};
    in >> std::ws >> denom;
    in >> DelimIO{':'} >> DelimIO{')'};

    if (in && denom != 0)
    {
      str.num = {numer, denom};
    }
    else
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::istream & operator>>(std::istream & in, Kavichki && str)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }

    return in >> std::quoted(str.str);
  }

  std::istream & operator>>(std::istream & in, DataStruct & str)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }

    DataStruct tmp{0, {0, 1}, ""};
    bool gotKey1 = false, gotKey2 = false, gotKey3 = false;

    in >> DelimIO{'('};
    while (in && (!gotKey1 || !gotKey2 || !gotKey3))
    {
      in >> DelimIO{':'};
      if (!in)
      {
        break;
      }

      std::string key;
      in >> key;
      if (!in)
      {
        break;
      }
      in >> std::ws;

      if (key == "key1" && !gotKey1)
      {
        in >> ULLIn{tmp.key1};
        if (in)
        {
          gotKey1 = true;
        }
      }
      else if (key == "key2" && !gotKey2)
      {
        in >> RacionalNum{tmp.key2};
        if (in)
        {
          gotKey2 = true;
        }
      }
      else if (key == "key3" && !gotKey3)
      {
        in >> Kavichki{tmp.key3};
        if (in)
        {
          gotKey3 = true;
        }
      }
      else
      {
        in.setstate(std::ios::failbit);
        break;
      }
    }

    in >> StringMatch{":)"};
    if (in && gotKey1 && gotKey2 && gotKey3)
    {
      str = tmp;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::ostream & operator<<(std::ostream & out, const DataStruct & str)
  {
    std::ostream::sentry s(out);
    if (!s)
    {
      return out;
    }

    out << "(:key1 " << str.key1 << "ull"
      << ":key2 (:N " << str.key2.first << ":D " << str.key2.second << ":)"
      << ":key3 " << std::quoted(str.key3) << ":)";
    return out;
  }

  bool operator<(const DataStruct & lhs, const DataStruct & rhs)
  {
    if (lhs.key1 != rhs.key1)
    {
      return lhs.key1 < rhs.key1;
    }

    long double lval = static_cast< long double >(lhs.key2.first) / lhs.key2.second;
    long double rval = static_cast< long double >(rhs.key2.first) / rhs.key2.second;
    if (lval != rval)
    {
      return lval < rval;
    }
    return lhs.key3.size() < rhs.key3.size();
  }
}
