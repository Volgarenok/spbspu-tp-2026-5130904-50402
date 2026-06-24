#ifndef KNOWLEDGE_BASE_HPP
#define KNOWLEDGE_BASE_HPP

#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_map>
#include "Note.hpp"

namespace samarin {
  class KnowledgeBase {
  public:
    void note(const std::string &name);
    void line(const std::string &name, const std::string &text);
    void show(const std::string &name, std::ostream &out) const;
    void drop(const std::string &name);

  private:
    std::unordered_map< std::string, std::shared_ptr< Note > > notes_;

    const std::shared_ptr< Note > &require(const std::string &name) const;
  };
}

#endif
