#ifndef NOTE_HPP
#define NOTE_HPP

#include <memory>
#include <string>
#include <vector>

namespace samarin {
  class Note {
  public:
    explicit Note(const std::string &name);

    const std::string &getName() const;
    void addLine(const std::string &line);
    const std::vector< std::string > &getLines() const;

    void link(const std::shared_ptr< Note > &target);
    void unlink(const std::shared_ptr< Note > &target);
    std::vector< std::string > mind() const;
    std::size_t countExpired() const;
    void refresh();

  private:
    using LinkList = std::vector< std::weak_ptr< Note > >;

    std::string name_;
    std::vector< std::string > lines_;
    LinkList links_;

    LinkList::const_iterator findLink(const std::shared_ptr< Note > &target) const;
  };
}

#endif
