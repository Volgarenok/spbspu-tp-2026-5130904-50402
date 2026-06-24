#include "KnowledgeBase.hpp"

#include <ostream>
#include <stdexcept>
#include <vector>

const std::shared_ptr< samarin::Note > &samarin::KnowledgeBase::require(const std::string &name) const
{
  const auto found = notes_.find(name);
  if (found == notes_.end()) {
    throw std::invalid_argument("no such note");
  }
  return found->second;
}
