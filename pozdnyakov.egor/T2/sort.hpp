#ifndef SORT_HPP
#define SORT_HPP

#include <cstddef>
#include <utility>

namespace pozdnyakov
{

  template< class FwdIt, class Cmp >
  std::size_t sort(FwdIt begin, FwdIt end, Cmp cmp)
  {
    if (begin == end) {
      return 0;
    }

    FwdIt pivot = begin;
    FwdIt i = begin;
    FwdIt j = begin;
    ++j;

    std::size_t elementsCount = 1;

    while (j != end) {
      if (cmp(*j, *pivot)) {
        ++i;
        std::swap(*i, *j);
      }
      ++j;
      ++elementsCount;
    }

    std::swap(*i, *pivot);

    pozdnyakov::sort(begin, i, cmp);

    FwdIt rightBegin = i;
    ++rightBegin;
    pozdnyakov::sort(rightBegin, end, cmp);

    return elementsCount;
  }

}

#endif
