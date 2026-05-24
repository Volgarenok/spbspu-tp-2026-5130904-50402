#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include "note.hpp"
#include "NoteTools.hpp"

void note(std::ostream&, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name;
  in >> name;
  if (vishnevskiy::findByName(name, vec) != -1)
  {
    throw std::logic_error("Note exists");
  }
  vec.push_back(std::make_shared<vishnevskiy::Note>(name, std::vector<std::string>(), std::vector<std::weak_ptr<vishnevskiy::Note>>()));
}

void line(std::ostream&, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name, text;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  if (ind != -1)
  {
    in >> std::quoted(text);
    vec[ind]->lines.push_back(text);
  }
  else
  {
    auto toignore = std::numeric_limits<std::streamsize>::max();
    in.ignore(toignore, '\n');
    throw std::logic_error("Cannot find name!");
  }
}

void show(std::ostream& out, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  bool hasOutput = false;
  std::string name;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  if (ind != -1)
  {
    for (size_t i = 0; i < vec[ind]->lines.size(); ++i)
    {
      hasOutput = true;
      out << vec[ind]->lines[i] << "\n";
    }
  }
  else
  {
    throw std::logic_error("Cannot find name!");
  }
  if (!hasOutput)
  {
    out << "\n";
  }
}

void drop(std::ostream&, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  if (ind != -1)
  {
    vec.erase(vec.begin() + ind);
  }
  else
  {
    throw std::logic_error("Cannot find name!");
  }
}

void link(std::ostream&, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name1;
  in >> name1;
  int ind1 = vishnevskiy::findByName(name1, vec);
  std::string name2;
  in >> name2;
  int ind2 = vishnevskiy::findByName(name2, vec);
  if (ind1 != -1 && ind2 != -1)
  {
    if (vishnevskiy::findLink(name2, vec[ind1]->links) != -1)
    {
      throw std::logic_error("Link exists");
    }
    vec[ind1]->links.push_back(vec[ind2]);
  }
  else
  {
    throw std::logic_error("Cannot find name!");
  }
}

void mind(std::ostream& out, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name;
  bool hasOutput = false;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  if (ind != -1)
  {
    for (size_t i = 0; i < vec[ind]->links.size(); ++i)
    {
      if (auto link = vec[ind]->links[i].lock())
      {
        hasOutput = true;
        out << link->name << "\n";
      }
    }
  }
  else
  {
    throw std::logic_error("Cannot find name!");
  }
  if (!hasOutput)
  {
    out << "\n";
  }
}

void halt(std::ostream&, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name;
  in >> name;
  std::string to_delete;
  in >> to_delete;
  int ind = vishnevskiy::findByName(name, vec);
  int link = vishnevskiy::findLink(to_delete, vec[ind]->links);
  if (ind != -1)
  {
    if (link != -1)
    {
      vec[ind]->links.erase(vec[ind]->links.begin() + link);
    }
    else
    {
      throw std::logic_error("Link not found");
    }
  }
  else
  {
    throw std::logic_error("Cannot find name!");
  }
}

void expired(std::ostream& out, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  size_t res = 0;
  if (ind != -1)
  {
    for (size_t i = 0; i < vec[ind]->links.size(); ++i)
    {
      if (vec[ind]->links[i].expired())
      {
        res++;
      }
    }
    out << res << "\n";
  }
  else
  {
    throw std::logic_error("Cannot find name!");
  }
}

void refresh(std::ostream&, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  if (ind != -1)
  {
    for (size_t i = vec[ind]->links.size(); i > 0; --i)
    {
      if (vec[ind]->links[i - 1].expired())
      {
        vec[ind]->links.erase(vec[ind]->links.begin() + (i - 1));
      }
    }
  }
  else
  {
    throw std::logic_error("Cannot find name!");
  }
}

int main()
{
  using cm_t = void(*)(std::ostream&, std::istream&, std::vector<std::shared_ptr<vishnevskiy::Note>>&);
  std::vector<std::shared_ptr<vishnevskiy::Note>> notes;
  std::map<std::string, cm_t> cmds;
  cmds["note"] = note;
  cmds["line"] = line;
  cmds["show"] = show;
  cmds["drop"] = drop;
  cmds["link"] = link;
  cmds["halt"] = halt;
  cmds["mind"] = mind;
  cmds["expired"] = expired;
  cmds["refresh"] = refresh;
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cout, std::cin, notes);
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "Bad input!\n";
    return 1;
  }
  return 0;
}

