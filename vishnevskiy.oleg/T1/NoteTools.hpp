#ifndef NOTE_HPP
#define NOTE_HPP
#include <string>
#include <vector>
#include <memory>
#include "note.hpp"

namespace vishnevskiy
{
  template <class T>
  int findByName(std::string name, std::vector<vishnevskiy::Note>* vec)
  {
    for (size_t i = 0; i < vec.size(); ++i)
    {
      if (vec[i].name == name)
      {
        return i;
      }
    }
    return -1;
  }
}

#endif
