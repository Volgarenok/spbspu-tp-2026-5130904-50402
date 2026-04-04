#ifndef NOTE_HPP
#define NOTE_HPP

#include <vector>
#include <string>
#include <memory>

namespace bukreev
{
  class Note
  {
  public:
    Note(std::string name) :
      mName(name) {}
    std::string getName();
    void appendLine(std::string line);
    void addLink(std::shared_ptr< Note > to);
    void show(std::ostream& out);

  private:
    std::string mName;
    std::vector< std::string > mLines;

    using NoteLink = std::pair< std::string, std::weak_ptr< Note > >;
    std::vector< NoteLink > mLinks;
  };
}

#endif
