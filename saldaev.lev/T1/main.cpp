#include <iomanip>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace saldaev
{
  struct Note
  {
    std::string name;

    Note(std::string name);
    Note(const Note &) = delete;
    Note(Note &&) = delete;
    ~Note() = default;
    Note &operator=(const Note &) = delete;
    Note &operator=(Note &&) = delete;

    void addLine(std::string line);
    bool addLink(std::shared_ptr< Note > note);
    bool removeLink(std::shared_ptr< Note > note);

    std::vector< std::weak_ptr< Note > > &getAllLinks();
    std::vector< std::shared_ptr< Note > > getActiveLinks() const;
    void show(std::ostream &) const;

  private:
    std::vector< std::string > lines_;
    std::vector< std::weak_ptr< Note > > links_;
  };

  void handleNote(std::istream &in, std::ostream &out, NoteMap &notes);
  void handleLine(std::istream &in, std::ostream &out, NoteMap &notes);
  void handleShow(std::istream &in, std::ostream &out, NoteMap &notes);
  void handleDrop(std::istream &in, std::ostream &out, NoteMap &notes);
}

using NoteMap = std::unordered_map< std::string, std::shared_ptr< saldaev::Note > >;
using cmd_t = void (*)(std::istream &, std::ostream &, NoteMap);

int main()
{}

saldaev::Note::Note(std::string name):
  name(name)
{}

void saldaev::Note::addLine(std::string line)
{
  lines_.push_back(line);
}

bool saldaev::Note::addLink(std::shared_ptr< Note > note)
{
  for (const auto &link : links_) {
    if (note == link.lock()) {
      return false;
    }
  }
  links_.push_back(note);
  return true;
}

bool saldaev::Note::removeLink(std::shared_ptr< Note > note)
{
  for (size_t i = 0; i < links_.size(); i++) {
    if (links_[i].lock() == note) {
      links_.erase(links_.begin() + i);
      return true;
    }
  }
  return false;
}

void saldaev::Note::show(std::ostream &out) const
{
  out << name << ":\n";
  for (const auto &line : lines_) {
    out << line << '\n';
  }
}

std::vector< std::weak_ptr< saldaev::Note > > &saldaev::Note::getAllLinks()
{
  return links_;
}

std::vector< std::shared_ptr< saldaev::Note > > saldaev::Note::getActiveLinks() const
{
  std::vector< std::shared_ptr< saldaev::Note > > ret;
  for (const auto &link : links_) {
    if (auto shared = link.lock()) {
      ret.push_back(shared);
    }
  }
  return ret;
}

void saldaev::handleNote(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  in >> name;

  if (notes.find(name) == notes.end()) {
    notes[name] = std::make_shared< saldaev::Note >(name);
  } else {
    out << "<INVALID COMMAND>\n";
  }
}

void saldaev::handleLine(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  std::string line;
  in >> name >> std::quoted(line);

  if (notes.find(name) == notes.end()) {
    out << "<INVALID COMMAND>\n";
  } else {
    notes[name]->addLine(line);
  }
}

void saldaev::handleShow(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  in >> name;

  if (notes.find(name) == notes.end()) {
    out << "<INVALID COMMAND>\n";
  } else {
    notes[name]->show(out);
  }
}

void saldaev::handleDrop(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  in >> name;

  auto it = notes.find(name);
  if (it != notes.end()) {
    notes.erase(it);
  } else {
    out << "<INVALID COMMAND>\n";
  }
}
