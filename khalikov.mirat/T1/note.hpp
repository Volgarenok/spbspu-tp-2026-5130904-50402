#ifndef NOTE_HPP
#define NOTE_HPP
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include <memory>

namespace khalikov
{

  struct Note
  {
    std::string name;
    std::vector< std::string > text;
    std::vector< std::weak_ptr< Note > > links;

    Note(const std::string &nname):
      name(nname)
    {}
  };

  using d_t = std::unordered_map< std::string, std::shared_ptr< Note > >;

  void noteCommand(std::istream &in, std::ostream &, d_t &data)
  {
    std::string name;
    in >> name;
    if (data.find(name) == data.cend()) {
      data.insert({name, std::make_shared< Note >(name)});
    } else {
      throw std::logic_error("Note already exist.");
    }
  }

  void lineCommand(std::istream &in, std::ostream &, d_t &data)
  {
    std::string name, str;
    in >> name;
    try {
      auto &note = data.at(name);
      in >> std::quoted(str);
      note->text.push_back(str);
    } catch (const std::out_of_range &) {
      throw std::logic_error("Note with this name doesn't exist.");
    }
  }

  void showCommand(std::istream &in, std::ostream &out, d_t &data)
  {
    std::string name;
    in >> name;
    try {
      auto &note = data.at(name);
      if (!note->text.empty()) {
        out << note->text.front();
        for (size_t i = 1; i < note->text.size(); ++i) {
          out << '\n' << note->text[i];
        }
      }
    } catch (const std::out_of_range &) {
      throw std::logic_error("Note with this name doesn't exist.");
    }
  }

  void dropCommand(std::istream &in, std::ostream &, d_t &data)
  {
    std::string name;
    in >> name;
    try {
      data.at(name);
      data.erase(name);
    } catch (const std::out_of_range &) {
      throw std::logic_error("Note with this name doesn't exist.");
    }
  }

  void linkCommand(std::istream &in, std::ostream &, d_t &data)
  {
    std::string name, yaname;
    in >> name >> yaname;
    try {
      auto &note = data.at(name);
      auto &yanote = data.at(yaname);
      for (const auto &link : note->links) {
        std::shared_ptr< Note > ptr = link.lock();
        if (ptr && ptr->name == yaname) {
          throw std::logic_error("This note already linked");
        }
      }
      note->links.push_back(yanote);
    } catch (const std::out_of_range &) {
      throw std::logic_error("Note with this name doesn't exist.");
    }
  }

  void haltCommand(std::istream &in, std::ostream &, d_t &data)
  {
    std::string name, yaname;
    in >> name >> yaname;
    try {
      auto &note = data.at(name);
      data.at(yaname);
      for (auto itlink = note->links.begin(); itlink != note->links.end(); ++itlink) {
        if (itlink->lock() && itlink->lock()->name == yaname) {
          note->links.erase(itlink);
          return;
        }
      }
      throw std::logic_error("Note is not linked with this.");
    } catch (const std::out_of_range &) {
      throw std::logic_error("Note with this name doesn't exist.");
    }
  }

  void mindCommand(std::istream &in, std::ostream &out, d_t &data)
  {
    std::string name;
    in >> name;
    try {
      auto &note = data.at(name);
      std::vector< std::string > temp;
      for (const auto &link : note->links) {
        auto ptr = link.lock();
        if (ptr) {
          temp.push_back(ptr->name);
        }
      }
      if (!temp.empty()) {
        out << temp.front();
        for (size_t i = 1; i < temp.size(); ++i) {
          out << '\n' << temp[i];
        }
      }
    } catch (const std::out_of_range &) {
      throw std::logic_error("Note with this name doesn't exist.");
    }
  }

  void expiredCommand(std::istream &in, std::ostream &out, d_t &data)
  {
    std::string name;
    in >> name;
    try {
      auto &note = data.at(name);
      size_t count = 0;
      for (const std::weak_ptr< Note > &link : note->links) {
        if (link.expired()) {
          count++;
        }
      }
      out << count << '\n';
    } catch (const std::out_of_range &) {
      throw std::logic_error("Note with this name doesn't exist.");
    }
  }

  void refreshCommand(std::istream &in, std::ostream &, d_t &data)
  {
    std::string name;
    in >> name;
    try {
      auto &note = data.at(name);
      for (auto itlink = note->links.begin(); itlink != note->links.end();) {
        if (itlink->expired()) {
          itlink = note->links.erase(itlink);
        } else {
          ++itlink;
        }
      }
    } catch (const std::out_of_range &) {
      throw std::logic_error("Note with this name doesn't exist.");
    }
  }
}

#endif
