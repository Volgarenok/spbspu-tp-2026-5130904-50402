#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include "note.hpp"
#include "NoteTools.hpp"

void note(std::ostream& out, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name;
  in >> name;
  vec.push_back(std::make_shared<vishnevskiy::Note>(name, std::vector<std::string>(), std::vector<std::weak_ptr<vishnevskiy::Note>>()));
}

void line(std::ostream& out, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
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
    std::cerr << "Cannot find name!\n";
  }
}

void show(std::ostream& out, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  if (ind != -1)
  {
    for (size_t i = 0; i < vec[ind]->lines.size(); ++i)
    {
      std::cout << vec[ind]->lines[i] << "\n";
    }
  }
  else
  {
    std::cerr << "Cannot find name!\n";
  }
}

void drop(std::ostream& out, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
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
    std::cerr << "Cannot find name!\n";
  }
}

void link(std::ostream& out, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name1;
  in >> name1;
  int ind1 = vishnevskiy::findByName(name1, vec);
  std::string name2;
  in >> name2;
  int ind2 = vishnevskiy::findByName(name2, vec);
  if (ind1 != -1 && ind2 != -1)
  {
    vec[ind1]->links.push_back(vec[ind2]);
  }
  else
  {
    std::cerr << "Cannot find name!\n";
  }
}

void mind(std::ostream& out, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  if (ind != -1)
  {
    for (size_t i = 0; i < vec[ind]->links.size(); ++i)
    {
      if (auto link = vec[ind]->links[i].lock())
      {
        out << link->name << "\n";
      }
    }
  }
  else
  {
    std::cerr << "Cannot find name!\n";
  }
}

void halt(std::ostream& out, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
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
      std::cerr << "Link not found\n";
    }
  }
  else
  {
    std::cerr << "Cannot find name!\n";
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
  }
  else
  {
    std::cerr << "Cannot find name!\n";
  }
}

void refresh(std::ostream& out, std::istream& in, std::vector<std::shared_ptr<vishnevskiy::Note>>& vec)
{
  std::string name;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  if (ind != -1)
  {
    for (size_t i = 0; i < vec[ind]->links.size(); ++i)
    {
      if (vec[ind]->links[i].expired())
      {
        vec[ind]->links.erase(vec[ind]->links.begin() + i);
      }
    }
  }
  else
  {
    std::cerr << "Cannot find name!\n";
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
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "Bad input!\n";
    return 1;
  }
  return 0;
}

