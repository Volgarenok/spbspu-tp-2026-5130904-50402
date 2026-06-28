#ifndef NOTES_HPP
#define NOTES_HPP
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace saldaev
{
  struct Note
  {
    Note(std::string name);

    void addLine(std::string line);
    void addLink(std::shared_ptr< Note > note);
    void removeLink(std::shared_ptr< Note > note);

    const std::string &getName();
    std::vector< std::weak_ptr< Note > > &getAllLinks();
    std::vector< std::weak_ptr< Note > > getActiveLinks() const;
    const std::vector< std::string > &getLines() const;

  private:
    std::string name_;
    std::vector< std::string > lines_;
    std::vector< std::weak_ptr< Note > > links_;
  };

  using noteMap = std::unordered_map< std::string, std::shared_ptr< saldaev::Note > >;
  using cmd_t = void (*)(std::istream &, std::ostream &, noteMap &);

  void handleNote(std::istream &in, std::ostream &out, noteMap &notes);
  void handleLine(std::istream &in, std::ostream &out, noteMap &notes);
  void handleShow(std::istream &in, std::ostream &out, noteMap &notes);
  void handleDrop(std::istream &in, std::ostream &out, noteMap &notes);

  void handleLink(std::istream &in, std::ostream &out, noteMap &notes);
  void handleHalt(std::istream &in, std::ostream &out, noteMap &notes);
  void handleMind(std::istream &in, std::ostream &out, noteMap &notes);
  void handleExpired(std::istream &in, std::ostream &out, noteMap &notes);
  void handleRefresh(std::istream &in, std::ostream &out, noteMap &notes);
}

#endif
