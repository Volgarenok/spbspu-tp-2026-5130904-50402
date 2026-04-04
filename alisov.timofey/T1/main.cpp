#include <iostream>
#include <unordered_map>

namespace alisov
{
  struct Note
  {
  };

  void note(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void line(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void show(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void drop(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void link(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void halt(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void mind(std::istream &in, std::ostream &out,
            std::unordered_map< std::string, alisov::Note > notes);
  void expired(std::istream &in, std::ostream &out,
               std::unordered_map< std::string, alisov::Note > notes);
  void refresh(std::istream &in, std::ostream &out,
               std::unordered_map< std::string, alisov::Note > notes);
}

int main()
{
}