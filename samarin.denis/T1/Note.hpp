#ifndef NOTE_HPP
#define NOTE_HPP

#include <string>
#include <vector>

namespace samarin {
  class Note {
  public:
    explicit Note(const std::string &name);

    const std::string &getName() const;
    void addLine(const std::string &line);
    const std::vector< std::string > &getLines() const;

  private:
    std::string name_;
    std::vector< std::string > lines_;
  };
}

#endif
