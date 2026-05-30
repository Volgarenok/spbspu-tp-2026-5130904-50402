#ifndef NOTE_HPP
#define NOTE_HPP
#include <cstddef>
#include <string>
#include <vector>
#include <memory>

namespace lavrentev
{
  struct Note
  {
    std::string name;
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< lavrentev::Note > > ptrs;
    Note() = default;
    explicit Note(const std::string& n) : name(n) {}
  };
}

#endif
