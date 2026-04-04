#ifndef NOTE_HPP
#define NOTE_HPP
#include <string>
#include <vector>
#include <memory>
namespace sogdanov
{
  struct Note
  {
    std::string name;
    std::vector<std::string> lines;
    std::vector<std::weak_ptr<Note>> links;
  };
}
#endif
