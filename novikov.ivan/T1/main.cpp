#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <limits>
#include <memory>
#include <vector>

namespace novikov
{
  struct Note;
}

using map_t = std::unordered_map< std::string, std::shared_ptr< novikov::Note > >;
using links_t = std::vector< std::pair< std::string, std::weak_ptr< novikov::Note > > >;

namespace novikov
{
  struct Note
  {
    std::vector< std::string > entries;
    links_t links;
  };

  void note(std::istream& in, std::ostream&, map_t& notes);
  void line(std::istream& in, std::ostream&, map_t& notes);
  void show(std::istream& in, std::ostream& out, map_t& notes);
  void drop(std::istream& in, std::ostream&, map_t& notes);
  void link(std::istream& in, std::ostream&, map_t& notes);
  void halt(std::istream& in, std::ostream&, map_t& notes);
  void mind(std::istream& in, std::ostream& out, map_t& notes);
  void expired(std::istream& in, std::ostream& out, map_t& notes);
  void refresh(std::istream& in, std::ostream&, map_t& notes);
  bool exist(const links_t& v, const std::string& s);
}

int main()
{
  map_t notes;

  using cmd_t = void (*)(std::istream&, std::ostream&, map_t&);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = novikov::note;
  cmds["line"] = novikov::line;
  cmds["show"] = novikov::show;
  cmds["drop"] = novikov::drop;
  cmds["link"] = novikov::link;
  cmds["halt"] = novikov::halt;
  cmds["mind"] = novikov::mind;
  cmds["expired"] = novikov::expired;
  cmds["refresh"] = novikov::refresh;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, notes);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toIgnore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toIgnore, '\n');
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}

void novikov::note(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteName;
  in >> noteName;
  if (notes.find(noteName) == notes.end())
  {
    notes[noteName] = std::make_shared< Note >();
  }
  else
  {
    throw std::logic_error("");
  }
}

void novikov::line(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteName, quotedText;
  in >> noteName >> std::quoted(quotedText);
  notes.at(noteName)->entries.push_back(quotedText);
}

void novikov::show(std::istream& in, std::ostream& out, map_t& notes)
{
  std::string noteName;
  in >> noteName;
  for (std::string line : notes.at(noteName)->entries)
  {
    out << line << '\n';
  }
  if (notes.at(noteName)->entries.empty())
  {
    out << '\n';
  }
}

void novikov::drop(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteName;
  in >> noteName;
  notes.at(noteName);
  notes.erase(noteName);
}

void novikov::link(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteTo, noteFrom;
  in >> noteFrom >> noteTo;
  std::shared_ptr< Note > toPtr = notes.at(noteTo);
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  if (!exist(fromPtr->links, noteTo))
  {
    fromPtr->links.push_back({noteTo, toPtr});
  }
  else
  {
    throw std::logic_error("");
  }
}

void novikov::halt(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteTo, noteFrom;
  in >> noteFrom >> noteTo;
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  notes.at(noteTo);
  for (auto it = fromPtr->links.begin(); it != fromPtr->links.end(); ++it)
  {
    if ((*it).first == noteTo)
    {
      fromPtr->links.erase(it);
      break;
    }
  }
}

void novikov::mind(std::istream& in, std::ostream& out, map_t& notes)
{
  std::string noteFrom;
  in >> noteFrom;
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  bool flag = false;
  for (const auto& pair : fromPtr->links)
  {
    if (pair.second.lock())
    {
      out << pair.first << '\n';
      flag = true;
    }
  }
  if (!flag)
  {
    out << '\n';
  }
}

void novikov::expired(std::istream& in, std::ostream& out, map_t& notes)
{
  std::string noteFrom;
  in >> noteFrom;
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  size_t k = 0;
  for (const auto& pair : fromPtr->links)
  {
    if (!pair.second.lock())
    {
      ++k;
    }
  }
  out << k << '\n';
}

void novikov::refresh(std::istream& in, std::ostream&, map_t& notes)
{
  std::string noteFrom;
  in >> noteFrom;
  std::shared_ptr< Note > fromPtr = notes.at(noteFrom);
  links_t newLinksVec;
  for (auto pair : fromPtr->links)
  {
    if (pair.second.lock())
    {
      newLinksVec.push_back(pair);
    }
  }
  fromPtr->links = newLinksVec;
}

bool novikov::exist(const links_t& v, const std::string& s)
{
  for (size_t i = 0; i < v.size(); ++i)
  {
    if (v[i].first == s)
    {
      return true;
    }
  }
  return false;
}
