#ifndef NOTE_HPP
#define NOTE_HPP
#include <string>
#include <vector>
#include <memory>

namespace vishnevskiy
{
  class Note
  {
    public:
      std::string name;
      std::vector<std::string> lines;
      std::vector<std::weak_ptr<Note>> links;
      Note() = default;
      Note(const std::string& n,
       const std::vector<std::string>& l,
       const std::vector<std::weak_ptr<Note>>& lnks): name(n), lines(l), links(lnks) {}
  };
}

#endif
