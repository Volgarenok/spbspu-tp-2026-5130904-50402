#include "notes.hpp"

saldaev::Note::Note(std::string name):
  name_(name)
{}

void saldaev::Note::addLine(std::string line)
{
  lines_.push_back(line);
}

void saldaev::Note::addLink(std::shared_ptr< Note > note)
{
  for (const std::weak_ptr< Note > &link : links_) {
    if (note == link.lock()) {
      throw std::out_of_range("");
    }
  }
  links_.push_back(note);
}

void saldaev::Note::removeLink(std::shared_ptr< Note > note)
{
  for (size_t i = 0; i < links_.size(); i++) {
    if (links_[i].lock() == note) {
      links_.erase(links_.begin() + i);
      return;
    }
  }
  throw std::out_of_range("");
}

const std::vector< std::string > &saldaev::Note::getLines() const
{
  return lines_;
}

const std::string &saldaev::Note::getName()
{
  return name_;
}

std::vector< std::weak_ptr< saldaev::Note > > &saldaev::Note::getAllLinks()
{
  return links_;
}

std::vector< std::weak_ptr< saldaev::Note > > saldaev::Note::getActiveLinks() const
{
  std::vector< std::weak_ptr< saldaev::Note > > ret;
  for (const std::weak_ptr< Note > &link : links_) {
    if (!link.expired()) {
      ret.push_back(link);
    }
  }
  return ret;
}

void saldaev::handleNote(std::istream &in, std::ostream &, noteMap &notes)
{
  std::string name;
  in >> name;

  bool sucsess = notes.emplace(name, std::make_shared< saldaev::Note >(name)).second;
  if (!sucsess) {
    throw std::runtime_error("");
  }
}

void saldaev::handleLine(std::istream &in, std::ostream &, noteMap &notes)
{
  std::string name;
  std::string line;
  in >> name;
  in >> std::quoted(line);

  notes.at(name)->addLine(line);
}

void saldaev::handleShow(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string name;
  in >> name;

  std::vector< std::string >::const_iterator begin = notes.at(name)->getLines().cbegin();
  std::vector< std::string >::const_iterator end = notes.at(name)->getLines().cend();
  if (begin != end) {
    out << *begin;
    ++begin;
    for (; begin != end; ++begin) {
      out << '\n' << (*begin);
    }
  }
}

void saldaev::handleDrop(std::istream &in, std::ostream &, noteMap &notes)
{
  std::string name;
  in >> name;

  auto it = notes.find(name);
  if (it == notes.end()) {
    throw std::runtime_error("");
  } else {
    notes.erase(it);
  }
}

void saldaev::handleLink(std::istream &in, std::ostream &, noteMap &notes)
{
  std::string note_from;
  std::string note_to;
  in >> note_from >> note_to;

  notes.at(note_from)->addLink(notes.at(note_to));
}

void saldaev::handleHalt(std::istream &in, std::ostream &, noteMap &notes)
{
  std::string note_from;
  std::string note_to;
  in >> note_from >> note_to;

  notes.at(note_from)->removeLink(notes.at(note_to));
}

void saldaev::handleMind(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string note_from;
  in >> note_from;

  std::vector< std::weak_ptr< saldaev::Note > > activeLinks = notes.at(note_from)->getActiveLinks();
  std::vector< std::weak_ptr< saldaev::Note > >::const_iterator begin = activeLinks.cbegin();
  std::vector< std::weak_ptr< saldaev::Note > >::const_iterator end = activeLinks.cend();

  if (begin != end) {
    out << (*begin).lock().get()->getName();
    ++begin;
    for (; begin != end; ++begin) {
      out << '\n' << (*begin).lock().get()->getName();
    }
  }
}

void saldaev::handleExpired(std::istream &in, std::ostream &out, noteMap &notes)
{
  std::string note_from;
  in >> note_from;

  size_t res = 0;
  std::vector< std::weak_ptr< Note > > &links = notes.at(note_from)->getAllLinks();
  for (auto it = links.begin(); it != links.end(); ++it) {
    if (it->expired()) {
      ++res;
    }
  }
  out << res;
}

void saldaev::handleRefresh(std::istream &in, std::ostream &, noteMap &notes)
{
  std::string note_from;
  in >> note_from;

  std::vector< std::weak_ptr< Note > > &links = notes.at(note_from)->getAllLinks();
  for (auto it = links.begin(); it != links.end();) {
    if (it->expired()) {
      it = links.erase(it);
    } else {
      ++it;
    }
  }
}
