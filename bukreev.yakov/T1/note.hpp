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
    void appendLine(std::string line);
    void show(std::ostream& out);

  private:
    std::vector< std::string > mLines;
  };
}

#endif
