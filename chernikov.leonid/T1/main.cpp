#include "db.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include <functional>
#include <sstream>

using CommandHandler = void (*)(std::istream &, std::ostream &, chernikov::NoteDB &);

void handle_note(std::istream &in, std::ostream &, chernikov::NoteDB &db)
{
  std::string name;
  in >> name;
  if (!db.createNote(name))
  {
    throw std::logic_error("note already exists");
  }
}

void handle_line(std::istream &in, std::ostream &, chernikov::NoteDB &db)
{
  std::string name;
  in >> name;
  std::string text;
  in >> std::quoted(text);
  if (!db.addLineToNote(name, text))
  {
    throw std::logic_error("note not found");
  }
}

void handle_show(std::istream &in, std::ostream &out, chernikov::NoteDB &db)
{
  std::string name;
  in >> name;
  if (!db.showNote(name, out))
  {
    throw std::logic_error("note not found");
  }
}

void handle_drop(std::istream &in, std::ostream &, chernikov::NoteDB &db)
{
  std::string name;
  in >> name;
  if (!db.dropNote(name))
  {
    throw std::logic_error("note not found");
  }
}

void handle_link(std::istream &in, std::ostream &, chernikov::NoteDB &db)
{
  std::string from, to;
  in >> from >> to;
  if (!db.linkNotes(from, to))
  {
    throw std::logic_error("cannot link");
  }
}

void handle_halt(std::istream &in, std::ostream &, chernikov::NoteDB &db)
{
  std::string from, to;
  in >> from >> to;
  if (!db.haltLink(from, to))
  {
    throw std::logic_error("cannot halt");
  }
}

void handle_mind(std::istream &in, std::ostream &out, chernikov::NoteDB &db)
{
  std::string name;
  in >> name;
  if (!db.noteExists(name))
  {
    out << '\n';
    throw std::logic_error("note not found");
  }
  auto names = db.mindLinks(name);
  for (const auto &n : names)
  {
    out << n << '\n';
  }
}

void handle_expired(std::istream &in, std::ostream &out, chernikov::NoteDB &db)
{
  std::string name;
  bool hasParam = static_cast<bool>(in >> name);

  if (hasParam && !db.noteExists(name))
  {
    throw std::logic_error("note not found");
  }

  out << '\n'
      << db.expiredCount() << '\n';
}

void handle_refresh(std::istream &in, std::ostream &, chernikov::NoteDB &db)
{
  std::string name;
  if (in >> name)
  {
    if (!db.noteExists(name))
    {
      throw std::logic_error("note not found");
    }
  }
  db.refreshAll();
}

int main()
{
  chernikov::NoteDB db;

  std::unordered_map<std::string, CommandHandler> handlers;
  handlers["note"] = handle_note;
  handlers["line"] = handle_line;
  handlers["show"] = handle_show;
  handlers["drop"] = handle_drop;
  handlers["link"] = handle_link;
  handlers["halt"] = handle_halt;
  handlers["mind"] = handle_mind;
  handlers["expired"] = handle_expired;
  handlers["refresh"] = handle_refresh;

  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    if (cmd.empty())
    {
      continue;
    }

    try
    {
      auto it = handlers.find(cmd);
      if (it == handlers.end())
      {
        throw std::out_of_range("unknown command");
      }
      it->second(iss, std::cout, db);
    }
    catch (const std::out_of_range &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (const std::logic_error &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
  return 0;
}
