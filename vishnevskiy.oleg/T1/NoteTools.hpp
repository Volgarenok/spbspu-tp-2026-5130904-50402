#ifndef NOTETOOLS_HPP
#define NOTETOOLS_HPP
#include <string>
#include <vector>
#include <memory>
#include "note.hpp"

namespace vishnevskiy
{
  int findByName(const std::string& name, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec);
  int findLink(const std::string& name, std::vector<std::weak_ptr<vishnevskiy::Note>>& links);
}

#endif
