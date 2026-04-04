#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace strelnikov
{
	struct Note;
  using db_t = std::unordered_map< std::string, std::shared_ptr< Note > >;
  void addNote(std::istream &, db_t &);
  void addLine(std::istream &, db_t &);
  void show(std::istream &, std::ostream &, db_t &);
  void drop(std::istream &, db_t &);
  void link(std::istream &, db_t &);
  void showMindMap(std::ostream &, db_t);
  void halt(std::istream &, db_t &);
  void expired(std::istream &, db_t &);
  void refresh(std::istream &, db_t &);
}
