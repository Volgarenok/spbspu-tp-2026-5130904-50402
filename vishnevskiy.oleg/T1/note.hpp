#ifndef NOTE_HPP
#define NOTE_HPP
#include <string>
#include <vector>
#include <memory>

namespace vishnevskiy
{
  template <class T>
  class Note
  {
    std::string name;
    std::vector<std::string> lines;
    std::vector<std::weak_ptr<Note>> links;
  };
}

#endif
