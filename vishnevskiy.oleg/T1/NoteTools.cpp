#include <string>
#include <vector>
#include <memory>
#include "note.hpp"
#include "NoteTools.hpp"

namespace vishnevskiy
{
  int findByName(const std::string& name, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
  {
    for (size_t i = 0; i < vec.size(); ++i)
    {
      if (vec[i]->name == name)
      {
        return i;
      }
    }
    return -1;
  }
  int findLink(const std::string& name, std::vector<std::weak_ptr<vishnevskiy::Note>>& links)
  {
    for (size_t i = 0; i < links.size(); ++i)
    {
      if (links[i].lock()->name == name)
      {
        return i;
      }
    }
    return -1;
  }
}
