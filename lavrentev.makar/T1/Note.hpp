#ifndef NOTE_HPP
#define NOTE_HPP
#include <cstddef>
#include <string>
#include <vector>

namespace lavrentev{
  struct Note
  {
    //std::string name;
    std::vector<std::string> lines;
    std::vector<std::weak_ptr<lavrentev::Note>> ptrs;
  };
}

#endif