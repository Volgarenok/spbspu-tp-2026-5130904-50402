#ifndef NOTE_HPP
#define NOTE_HPP
#include <vector>
#include <string>
#include <memory>

namespace lavrentev
{
  struct Note
  {
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > ptrs;
    std::string name;

    Note() = default;
    explicit Note(const std::string& n);
  };
}

#endif
