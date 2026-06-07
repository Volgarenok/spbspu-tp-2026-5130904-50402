#include <iostream>
#include <limits>
#include "commands.hpp"

int main()
{
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      bukreev::executeCommand(cmd, std::cin, std::cout);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
