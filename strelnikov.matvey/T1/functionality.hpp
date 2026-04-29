#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace strelnikov
{
  struct Note;
  using It_t = std::vector< std::weak_ptr< Note > >::iterator;
  using db_t = std::unordered_map< std::string, std::shared_ptr< Note > >;
  void addNote(std::istream &, std::ostream &, db_t &);
  void addLine(std::istream &, std::ostream &, db_t &);
  void show(std::istream &, std::ostream &, db_t &);
  void drop(std::istream &, std::ostream &, db_t &);
  void link(std::istream &, std::ostream &, db_t &);
  void showMindMap(std::istream &, std::ostream &, db_t &);
  void halt(std::istream &, std::ostream &, db_t &);
  void expired(std::istream &, std::ostream &, db_t &);
  void refresh(std::istream &, std::ostream &, db_t &);
  It_t findLink(It_t start, It_t end, std::string name);
}
