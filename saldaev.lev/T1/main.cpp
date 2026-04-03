#include <iomanip>
#include <iostream>
#include <limits>
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

int main()
{
  saldaev::noteMap notes;
  std::unordered_map< std::string, saldaev::cmd_t > cmds;
  cmds["note"] = saldaev::handleNote;
  cmds["line"] = saldaev::handleLine;
  cmds["show"] = saldaev::handleShow;
  cmds["drop"] = saldaev::handleDrop;
  cmds["link"] = saldaev::handleLink;
  cmds["halt"] = saldaev::handleHalt;
  cmds["mind"] = saldaev::handleMind;
  cmds["expired"] = saldaev::handleExpired;
  cmds["refresh"] = saldaev::handleRefresh;

  std::string comand;
  while (std::cin >> comand) {
    try {
      cmds.at(comand)(std::cin, std::cout, notes);
    } catch (const std::out_of_range &) {
      std::cout << "<INVALID COMMAND>\n";
      std::streamsize toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
}

saldaev::Note::Note(std::string name):
  name(name)
{}

void saldaev::Note::addLine(std::string line)
{
  lines_.push_back(line);
}

bool saldaev::Note::addLink(std::shared_ptr< Note > note)
{
  for (const std::weak_ptr< Note > &link : links_) {
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
  for (const std::string &line : lines_) {
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
  for (const std::weak_ptr< Note > &link : links_) {
    if (std::shared_ptr< Note > shared = link.lock()) {
      ret.push_back(shared);
    }
  }
  return ret;
}

void saldaev::handleNote(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string name;
  in >> name;

  if (notes.find(name) == notes.end()) {
    notes[name] = std::make_shared< saldaev::Note >(name);
  } else {
    out << "<INVALID COMMAND>\n";
  }
}

void saldaev::handleLine(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string name;
  std::string line;
  in >> name;
  in >> std::quoted(line);

  if (notes.find(name) == notes.end()) {
    out << "<INVALID COMMAND>\n";
  } else {
    notes[name]->addLine(line);
  }
}

void saldaev::handleShow(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string name;
  in >> name;

  if (notes.find(name) == notes.end()) {
    out << "<INVALID COMMAND>\n";
  } else {
    notes[name]->show(out);
  }
}

void saldaev::handleDrop(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string name;
  in >> name;

  auto it = notes.find(name);
  if (it == notes.end()) {
    out << "<INVALID COMMAND>\n";
  } else {
    notes.erase(it);
  }
}

void saldaev::handleLink(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string note_from;
  std::string note_to;
  in >> note_from >> note_to;

  if ((notes.find(note_from) == notes.end()) || (notes.find(note_to) == notes.end())) {
    out << "<INVALID COMMAND>\n";
  } else {
    if (!notes[note_from]->addLink(notes[note_to])) {
      out << "<INVALID COMMAND>\n";
    }
  }
}

void saldaev::handleHalt(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string note_from;
  std::string note_to;
  in >> note_from >> note_to;

  if ((notes.find(note_from) == notes.end()) || (notes.find(note_to) == notes.end())) {
    out << "<INVALID COMMAND>\n";
  } else {
    if (!(notes[note_from]->removeLink(notes[note_to]))) {
      out << "<INVALID COMMAND>\n";
    }
  }
}

void saldaev::handleMind(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string note_from;
  in >> note_from;

  if (notes.find(note_from) == notes.end()) {
    out << "<INVALID COMMAND>\n";
  } else {
    for (const std::shared_ptr< Note > &link : notes[note_from]->getActiveLinks()) {
      out << link->name << '\n';
    }
  }
}

void saldaev::handleExpired(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string note_from;
  in >> note_from;

  size_t res = 0;
  if (notes.find(note_from) == notes.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::vector< std::weak_ptr< Note > > &links = notes[note_from]->getAllLinks();
  for (auto it = links.begin(); it != links.end(); ++it) {
    if (it->expired()) {
      ++res;
    }
  }
  out << res << '\n';
}

void saldaev::handleRefresh(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string note_from;
  in >> note_from;

  if (notes.find(note_from) == notes.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::vector< std::weak_ptr< Note > > &links = notes[note_from]->getAllLinks();
  for (auto it = links.begin(); it != links.end();) {
    if (it->expired()) {
      it = links.erase(it);
    } else {
      ++it;
    }
  }
}
