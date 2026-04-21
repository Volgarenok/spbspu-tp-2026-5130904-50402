#ifndef NOTE_HPP
#define NOTE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

namespace chernikov {

  class Note
  {
  public:
    explicit Note(std::string name);

    void addLine(const std::string &text);
    void show(std::ostream &out) const;
    bool addLink(std::shared_ptr< Note > target);
    void removeLink(const std::string &targetName);
    std::vector< std::string > getLinkedNames() const;
    size_t countExpiredLinks() const;
    void removeExpiredLinks();
    const std::string &name() const;

  private:
    std::string name_;
    std::vector< std::string > lines_;
    std::vector< std::weak_ptr< Note > > links_;
  };
}

#endif
