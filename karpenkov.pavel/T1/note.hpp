#ifndef NOTE_HPP
#define NOTE_HPP
#include <iostream>
#include <memory>
#include <vector>
#include <string>

namespace karpenkov
{
  class Note
  {
  public:
    Note(const std::string &name):
      name(name) {};
    void newLine(std::string &text)
    {
      lines.push_back(text);
    }
    void printNote(std::ostream &out)
    {
      for (size_t i = 0; i < lines.size(); ++i) {
        out << lines[i] << '\n';
      }
    }
    void createLink(std::shared_ptr< Note > to)
    {
      for (size_t i = 0; i < links.size(); ++i) {
        std::shared_ptr< Note > sp = links[i].lock();
        if (sp == to) {
          return;
        }
      }
      links.push_back(to);
    }
    void printLinks(std::ostream &out) const
    {
      for (const std::weak_ptr< Note > &link : links) {
        std::shared_ptr< Note > sp = link.lock();
        if (sp) {
          out << sp->name << '\n';
        }
      }
    }
    size_t countExpired() const
    {
      size_t count = 0;
      for (const std::weak_ptr< Note > &link : links) {
        if (link.expired()) {
          ++count;
        }
      }
      return count;
    }
    void refreshLinks()
    {
      auto it = links.begin();
      while (it != links.end()) {
        if (it->expired()) {
          it = links.erase(it);
        } else {
          ++it;
        }
      }
    }
    void removeLink(const std::shared_ptr< Note > &note)
    {
      auto it = links.begin();
      while (it != links.end()) {

        if (it->lock() == note) {
          links.erase(it);
          return;
        }
        ++it;
      }
    }

  private:
    std::string name;
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > links;
  };
}

#endif
