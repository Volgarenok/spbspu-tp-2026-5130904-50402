#include "operations.hpp"
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <cctype>

namespace karpenkov
{
  bool isOnlySpaces(const std::string &str)
  {
    for (char c : str) {
      if (!std::isspace(static_cast< unsigned char >(c))) {
        return false;
      }
    }
    return true;
  }
  void addNote(std::istream &in, std::ostream &, mapOfNotes &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::runtime_error("empty note name");
    }
    std::string tail;
    std::getline(in, tail);
    if (!isOnlySpaces(tail)) {
      throw std::runtime_error("extra arguments");
    }
    if (notes.find(name) != notes.cend()) {
      throw std::runtime_error("note with this name is already exist");
    }
    std::shared_ptr< Note > newNote = std::make_shared< Note >(name);
    notes[name] = newNote;
  }
  void addLine(std::istream &in, std::ostream &, mapOfNotes &notes)
  {
    std::string name, text;
    if (!(in >> name)) {
      throw std::runtime_error("empty arguments for line");
    }
    if (notes.find(name) == notes.cend()) {
      throw std::runtime_error("note with such name doesn't exist");
    }
    in >> std::quoted(text);
    if (!in) {
      throw std::runtime_error("invalid quoted string");
    }
    std::string tail;
    std::getline(in, tail);
    if (!isOnlySpaces(tail)) {
      throw std::runtime_error("extra arguments");
    }
    std::shared_ptr< Note > neededNote = notes.at(name);
    neededNote->newLine(text);
  }
  void dropNote(std::istream &in, std::ostream &, mapOfNotes &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::runtime_error("empty argument for deleting note");
    }
    std::string tail;
    std::getline(in, tail);
    if (!isOnlySpaces(tail)) {
      throw std::runtime_error("extra arguments");
    }
    if (notes.find(name) == notes.cend()) {
      throw std::runtime_error("note with such name doesn't exist");
    }
    notes.erase(name);
  }
  void showNote(std::istream &in, std::ostream &out, mapOfNotes &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::runtime_error("empty name argument for showNote");
    }
    std::string tail;
    std::getline(in, tail);
    if (!isOnlySpaces(tail)) {
      throw std::runtime_error("extra arguments");
    }
    if (notes.find(name) == notes.cend()) {
      throw std::runtime_error("note with such name doesn't exist");
    }
    std::shared_ptr< Note > neededNote = notes.at(name);
    neededNote->printNote(out);
  }
  void addLink(std::istream &in, std::ostream &, mapOfNotes &notes)
  {
    std::string noteTo, noteFrom;
    if (!(in >> noteFrom >> noteTo)) {
      throw std::runtime_error("missing argument");
    }
    std::string tail;
    std::getline(in, tail);
    if (!isOnlySpaces(tail)) {
      throw std::runtime_error("extra arguments");
    }
    if (notes.find(noteFrom) == notes.cend() || notes.find(noteTo) == notes.cend()) {
      throw std::runtime_error("no such note");
    }
    std::shared_ptr< Note > neededNote = notes.at(noteFrom);
    neededNote->createLink(notes.at(noteTo));
  }
  void showLinks(std::istream &in, std::ostream &out, mapOfNotes &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::runtime_error("missing argument");
    }
    std::string tail;
    std::getline(in, tail);
    if (!isOnlySpaces(tail)) {
      throw std::runtime_error("extra arguments");
    }
    if (notes.find(name) == notes.cend()) {
      throw std::runtime_error("note doesn't exist");
    }
    notes.at(name)->printLinks(out);
  }
  void showExpired(std::istream &in, std::ostream &out, mapOfNotes &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::runtime_error("missing argument");
    }
    std::string tail;
    std::getline(in, tail);
    if (!isOnlySpaces(tail)) {
      throw std::runtime_error("extra arguments");
    }
    if (notes.find(name) == notes.cend()) {
      throw std::runtime_error("note doesn't exist");
    }
    out << notes.at(name)->countExpired() << '\n';
  }
  void refreshLinks(std::istream &in, std::ostream &, mapOfNotes &notes)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::runtime_error("missing argument");
    }
    std::string tail;
    std::getline(in, tail);
    if (!isOnlySpaces(tail)) {
      throw std::runtime_error("extra arguments");
    }
    if (notes.find(name) == notes.cend()) {
      throw std::runtime_error("note doesn't exist");
    }
    notes.at(name)->refreshLinks();
  }
  void removeLink(std::istream &in, std::ostream &, mapOfNotes &notes)
  {
    std::string noteFrom;
    std::string noteTo;
    if (!(in >> noteFrom >> noteTo)) {
      throw std::runtime_error("missing argument");
    }
    std::string tail;
    std::getline(in, tail);
    if (!isOnlySpaces(tail)) {
      throw std::runtime_error("extra arguments");
    }
    if (notes.find(noteFrom) == notes.cend()) {
      throw std::runtime_error("note-from doesn't exist");
    }
    if (notes.find(noteTo) == notes.cend()) {
      throw std::runtime_error("note-to doesn't exist");
    }
    notes.at(noteFrom)->removeLink(notes.at(noteTo));
  }

}
