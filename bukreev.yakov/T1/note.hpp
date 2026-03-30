#ifndef NOTE_HPP
#define NOTE_HPP

#include <vector>
#include <string>

namespace bukreev
{
  class Note
  {
  public:
    Note() = default;

  private:
    std::vector< std::string > mLines;
  };
}

#endif
