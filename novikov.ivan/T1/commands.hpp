#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace novikov
{
  struct Note;
  using map_t = std::unordered_map< std::string, std::shared_ptr< novikov::Note > >;
  using links_t = std::vector< std::pair< std::string, std::weak_ptr< novikov::Note > > >;

  struct Note
  {
    std::vector< std::string > entries;
    links_t links;
  };

  void note(std::istream& in, std::ostream&, map_t& notes);
  void line(std::istream& in, std::ostream&, map_t& notes);
  void show(std::istream& in, std::ostream& out, map_t& notes);
  void drop(std::istream& in, std::ostream&, map_t& notes);
  void link(std::istream& in, std::ostream&, map_t& notes);
  void halt(std::istream& in, std::ostream&, map_t& notes);
  void mind(std::istream& in, std::ostream& out, map_t& notes);
  void expired(std::istream& in, std::ostream& out, map_t& notes);
  void refresh(std::istream& in, std::ostream&, map_t& notes);
  bool exist(const links_t& v, const std::string& s);
}

#endif
