#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include "note.hpp"
#include "NoteTools.hpp"

void note(std::ostream& out, std::istream& in, std::vector<vishnevskiy::Note>& vec)
{
  std::string name;
  in >> name;
  vec.push_back(vishnevskiy::Note{name, std::vector<std::string>(), std::vector<std::weak_ptr<Note>>()});
}

void line(std::ostream& out, std::istream& in, std::vector<vishnevskiy::Note>& vec)
{
  std::string name, text;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  if (ind != -1)
  {
    in >> std::quoted(text);
    vec[ind].lines.push_back(text);
  }
  else
  {
    std::cerr << "Cannot find name!\n";
  }
}

void show(std::ostream& out, std::istream& in, std::vector<vishnevskiy::Note>& vec)
{
  std::string name;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  if (ind != -1)
  {
    for (size_t i = 0; i < vec[ind].lines.size(); ++i)
    {
      std::cout << vec[ind].lines[i] << "\n";
    }
  }
  else
  {
    std::cerr << "Cannot find name!\n";
  }
}

void drop(std::ostream& out, std::istream& in, std::vector<vishnevskiy::Note>& vec)
{
  std::string name;
  in >> name;
  int ind = vishnevskiy::findByName(name, vec);
  if (ind != -1)
  {
    delete vec[ind];
  }
  else
  {
    std::cerr << "Cannot find name!\n";
  }
}

int main()
{
  using cm_t = void(*)(std::ostream&, std::istream&, std::vector<vishnevskiy::Note>&);
  std::vector<vishnevskiy::Note> notes;
  std::map<std::string, cm_t> cmds;
  cmds["add"] = add;
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, notes);
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, "\n");
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
