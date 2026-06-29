#include "commands.hpp"

#include <functional>
#include <iomanip>
#include <istream>
#include <limits>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>

namespace {
  using samarin::KnowledgeBase;
  using CommandHandler = std::function< void(KnowledgeBase &, std::istream &, std::ostream &) >;

  std::string readWord(std::istream &in)
  {
    std::string word;
    if (!(in >> word)) {
      throw std::invalid_argument("expected argument");
    }
    return word;
  }

  std::map< std::string, CommandHandler > makeHandlers()
  {
    std::map< std::string, CommandHandler > handlers;
    handlers["note"] = [](KnowledgeBase &base, std::istream &in, std::ostream &)
    {
      base.note(readWord(in));
    };
    handlers["line"] = [](KnowledgeBase &base, std::istream &in, std::ostream &)
    {
      const std::string name = readWord(in);
      std::string text;
      if (!(in >> std::quoted(text))) {
        throw std::invalid_argument("expected quoted text");
      }
      base.line(name, text);
    };
    handlers["show"] = [](KnowledgeBase &base, std::istream &in, std::ostream &out)
    {
      base.show(readWord(in), out);
    };
    handlers["drop"] = [](KnowledgeBase &base, std::istream &in, std::ostream &)
    {
      base.drop(readWord(in));
    };
    handlers["link"] = [](KnowledgeBase &base, std::istream &in, std::ostream &)
    {
      const std::string from = readWord(in);
      base.link(from, readWord(in));
    };
    handlers["halt"] = [](KnowledgeBase &base, std::istream &in, std::ostream &)
    {
      const std::string from = readWord(in);
      base.halt(from, readWord(in));
    };
    handlers["mind"] = [](KnowledgeBase &base, std::istream &in, std::ostream &out)
    {
      base.mind(readWord(in), out);
    };
    handlers["expired"] = [](KnowledgeBase &base, std::istream &in, std::ostream &out)
    {
      base.expired(readWord(in), out);
    };
    handlers["refresh"] = [](KnowledgeBase &base, std::istream &in, std::ostream &)
    {
      base.refresh(readWord(in));
    };
    return handlers;
  }
}

void samarin::processCommands(KnowledgeBase &base, std::istream &in, std::ostream &out)
{
  const std::map< std::string, CommandHandler > handlers = makeHandlers();
  std::string command;
  while (in >> command) {
    try {
      const auto found = handlers.find(command);
      if (found == handlers.end()) {
        throw std::invalid_argument("unknown command");
      }
      found->second(base, in, out);
    } catch (const std::exception &) {
      if (in.fail() && !in.eof()) {
        in.clear();
      }
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      out << "<INVALID COMMAND>\n";
    }
  }
}
