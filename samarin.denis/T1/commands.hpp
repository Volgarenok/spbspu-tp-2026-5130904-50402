#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include "KnowledgeBase.hpp"

namespace samarin {
  void processCommands(KnowledgeBase &base, std::istream &in, std::ostream &out);
}

#endif
