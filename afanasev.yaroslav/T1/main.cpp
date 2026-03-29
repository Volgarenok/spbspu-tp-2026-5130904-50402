#include <iostream>
#include <unordered_map>
#include "cmd.hpp"
#include "note.hpp"

#include <iosfwd>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

void hi()
{
  std::cout << "aaaaaaaaaAAAAAAAAAAaaaaaaaa" << '\n';
}

int main()
{
  afanasev::note_t db;

  const std::unordered_map< std::string, afanasev::func_t > cmds
  {

  };

  std::string cmd;

  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, db);
    }
    catch (const std::out_of_range &)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.ignore(afanasev::streamMax, '\n');
    }
    catch (const std::logic_error & e)
    {
      std::cout << e.what() << '\n';
    }

/*
    if (cmd == "hello")
    {
      hi();
    }
    else
    {
      std::cout << "not have this command" << '\n';
    }
*/
  }

  if (!std::cin.eof())
  {
    std::cerr << "Error input" << '\n';
    return 1;
  }

  return 0;
}
