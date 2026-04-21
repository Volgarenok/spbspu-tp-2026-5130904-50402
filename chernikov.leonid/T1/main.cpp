#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

#include <map>
#include <unordered_map>

void hi(std::istream &, std::ostream &out, std::vector< std::string > &)
{
  out << "Hello, username!\n";
}
void noop(std::istream &, std::ostream &, std::vector< std::string > &)
{
}

void add_string(std::istream &in, std::ostream &, std::vector< std::string > &db)
{
  std::string str;
  in >> std::quoted(str);
  db.push_back(str);
}

void show_last(std::istream &, std::ostream &out, std::vector< std::string > &db)
{
  if (db.empty())
  {
    throw std::logic_error("db is empty");
  }
  out << db.back() << "\n";
}

int main()
{
  std::vector< std::string > db;

  using cmd_t = void (*)(std::istream &, std::ostream &, std::vector< std::string > &);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["add"] = add_string;
  cmds["show-last"] = show_last;
  cmds["hello"] = hi;
  cmds["noop"] = noop;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, db);
    } catch (const std::out_of_range &)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error &e)
    {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
